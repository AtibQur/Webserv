#include "main.hpp"

namespace fs = std::filesystem;

void Client::openAndClose()
{
    if (m_serverToCgi.m_pipeFd[READ] != -1)
        close(m_serverToCgi.m_pipeFd[READ]);
    if (m_serverToCgi.m_pipeFd[WRITE] != -1)
        close(m_serverToCgi.m_pipeFd[WRITE]);
    if (m_cgiToServer.m_pipeFd[READ] != -1)
        close(m_cgiToServer.m_pipeFd[READ]);
    if (m_cgiToServer.m_pipeFd[WRITE] != -1)
        close(m_cgiToServer.m_pipeFd[WRITE]);
}

/* GET */
void Client::handleGetMethod()
{
    if (!_response.getResponseMessage().empty()) // Is CGI!
    {
        _response.setSocketFd(m_socketFd);
        _response.sendResponse();
        openAndClose();
        return;
    }

    Response clientResponse(m_socketFd, "200 OK");
    _response = clientResponse;

    std::string filePath;
    Location clientLocation = m_server.getConf()->getLocation(getUri());
    if (_isDir)
    {
        filePath = m_server.getConf()->getRoot() + "/" + _file_if_dir;
    }
    else if (clientLocation.getPath() == getUri())
        filePath = m_server.getConf()->getRoot() + clientLocation.getPath() + "/" + clientLocation.getIndex();
    else
        filePath = m_server.getConf()->getRoot() + getUri();
    if (fs::is_directory(filePath) && !_isDir)
    {
        std::ifstream htmlFile("");
        std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));
        fileContent += generateDirectoryListing(clientLocation.getPath());

        _response.setServerName(m_server.getConf()->getServerNames());
        _response.setContent("Content-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);
        htmlFile.close();
        _response.sendResponse();
        return;
    }
    std::ifstream htmlFile(filePath);
    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));

    if (!htmlFile.is_open())
    {
        _response.setContent("14\n\nFile not found");
    }
    else
    {
        if (clientLocation.getAutoIndex() && !_isDir)
        {
            fileContent += generateDirectoryListing(clientLocation.getPath());
        }
        _response.setServerName(m_server.getConf()->getServerNames());
        _response.setContent("Content-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);
    }
    _isDir = false;
    htmlFile.close();
    _response.sendResponse();
    openAndClose();
}

/* WHEN AUTOINDEX IS ON, LIST ALL DIRECTORIES ON THE SCREEN */
std::string Client::generateDirectoryListing(std::string dirPath)
{
    std::string listing;
    listing += "<h3>";
    listing += "<ul>";
    for (const auto &entry : fs::directory_iterator(m_server.getConf()->getRoot() + "/" + dirPath))
    {
        listing += "<li>";

        std::string fileName = entry.path().filename().string();
        std::string displayName = entry.path().stem().string();

        if (fs::is_directory(entry.path()))
        {
            listing += "[DIR] " + fileName;
            listing += generateDirectoryListing(dirPath + "/" + fileName);
        }
        else
        {
            listing += displayName;
        }
        listing += "</li>";
    }
    listing += "</ul>";
    listing += "</h3>";
    return listing;
}

/* POST */
void Client::handlePostMethod()
{
    std::string filePath;
    if (getFileNameBody().empty())
    {
        std::cout << "No file name" << std::endl;
        handleGetMethod();
    }
    Response clientResponse(m_socketFd, "302 FOUND");

    if (_location[_uri].getUploadPath().empty())
        filePath = m_server.getConf()->getRoot() + "/" + getFileNameBody();
    else
        filePath = m_server.getConf()->getRoot() + "/" + _location[_uri].getUploadPath() + "/" + getFileNameBody();

    if (filePath == m_server.getConf()->getRoot() + "/")
    {
        std::cerr << "Empty request" << std::endl;
        return;
    }
    std::ifstream htmlFile(filePath);
    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));
    if (!htmlFile.is_open())
    {
        std::cerr << "Error opening file" << std::endl;
    }
    clientResponse.setContent("Location: " + _location[_uri].getUploadPath() + "/" + getFileNameBody() + "\n\n");
    clientResponse.setContent("Content-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);
    clientResponse.sendResponse();
    htmlFile.close();
}

/* DELETE */
void Client::handleDeleteMethod()
{
    std::string filePath;
    if (_location[_uri].getUploadPath().empty())
        filePath = m_server.getConf()->getRoot() + "/" + getFileNameBody();
    else
        filePath = m_server.getConf()->getRoot() + "/" + _location[_uri].getUploadPath() + "/" + getFileNameBody();
    if (fs::exists(filePath))
    {
        if (filePath == m_server.getConf()->getRoot() + "/" + _location[_uri].getUploadPath() + "/")
        {
            handleGetMethod();
            return ;
        }
        fs::remove(filePath); 
        std::cout << "File deleted successfully." << std::endl;
        Response goodResponse(m_socketFd, "202 Accepted");

        std::ifstream htmlFile("docs/response_pages/delete.html");
        std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));
        goodResponse.setContent("Content-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);

        goodResponse.sendResponse();
        return ;
    }
    else
    {
        std::cout << "File does not exist." << std::endl;
        Response goodResponse(m_socketFd, "204 No Content");
        goodResponse.sendResponse();
        return ;
    }
}
