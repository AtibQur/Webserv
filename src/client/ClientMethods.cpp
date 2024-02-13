#include "../../inc/main.hpp"

namespace fs = std::filesystem;

/* REQUEST CHECK IF THE URI IS IN LOCATIONS AND IF THE METHOD IS ALLOWED*/
bool Client::checkPathAndMethod()
{
    Location clientLocation = m_server.getConf()->getLocation(getUri());

    /* CGI */
    if (getUri().find(".py") != std::string::npos){
        // executeCgi();
		if (handleCGI() == 1) {
            throw (std::invalid_argument("500 Internal server error"));
        }
        if (handleCGI() == 2) {
            throw std::invalid_argument("404 Not Found");
        }
        _isCgi = true;
        return true;
	}
    if (getUri() == "/teapot")
    {
        throw std::invalid_argument("418 I'm a teapot");
    }
    std::cout << clientLocation.getPath() << std::endl;
    std::cout << getUri() << std::endl;
    if (!fs::exists("root" + getUri()))
    {
        std::cout << "1" << std::endl;
        throw std::invalid_argument("404 Not Found");
    }
    if (fs::is_regular_file("root" + getUri()))
        return true;
    if (fs::is_directory("root" + getUri()) && clientLocation.getPath() != getUri())
    {
        std::cout << "2" << std::endl;
        _isDir = true;
        if (_file_if_dir.empty())
        {
            throw std::invalid_argument("404 Not Found"); // Set a default file to answer if the request is a directory
        }
    }
    if (clientLocation.getIndex().empty() && clientLocation.getAutoIndex() == false)
    {
        std::cout << "3" << std::endl;
        throw std::invalid_argument("404 Not Found");
    }
    if (clientLocation.getPath().empty())
    {
        std::cout << "4" << std::endl;
        throw std::invalid_argument("404 Not Found");
    }
    if ("/root/" + access(getUri().c_str(), R_OK) == 0)
    {
        std::cout << "error after" << "\n";
        return true;
    }

    std::vector<std::string> methods = clientLocation.getMethods();
    if (methods.empty())
    {
        throw std::invalid_argument("405 Method Not Allowed");
    }
    std::vector<std::string>::iterator it = methods.begin();
    for (it; it < methods.end(); it++)
    {
        if (getMethod() == *it)
        {
            return true;
        }
    }
    if (it == methods.end())
    {
        throw std::invalid_argument("405 Method Not Allowed");
    }
    throw std::invalid_argument("400 Bad Request");
}

void Client::handleResponse()
{
    if (_response.getCode().empty())
    {
        int method = getNbMethod();
        switch (method)
        {
        case 1:
            handleGetMethod();
            break;
        case 2:
            handlePostMethod();
            break;
        case 3:
            handleDeleteMethod();
            break;
        default:
            std::cout << "Default Method" << std::endl;
        }
    }
    else
    {
        std::cout << "Creating Error Response" << std::endl;
        createErrorResponse();
    }
    modifyEpoll(this, EPOLLIN, getSocketFd());
    Response empty;
    _response = empty;
}

/* GET */
void Client::handleGetMethod()
{
    Response clientResponse(m_socketFd, "200 OK");

    std::string filePath;
    Location clientLocation = m_server.getConf()->getLocation(getUri());
    if (_isDir)
    {
        filePath = "root/" + _file_if_dir;
    }
    else if (clientLocation.getPath() == getUri())
        filePath = "root" + clientLocation.getPath() + "/" + clientLocation.getIndex();
    else
        filePath = "root" + getUri();

    if (fs::is_directory(filePath) && !_isDir)
    {
        std::ifstream htmlFile("");
        std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));
        fileContent += generateDirectoryListing(clientLocation.getPath());
        clientResponse.setContent("Content-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);
        htmlFile.close();
        clientResponse.sendResponse();
        return;
    }
    std::ifstream htmlFile(filePath);
    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));

    if (!htmlFile.is_open())
    {
        clientResponse.setContent("14\n\nFile not found");
    }
    else
    {
        if (clientLocation.getAutoIndex() && !_isDir)
        {
            fileContent += generateDirectoryListing(clientLocation.getPath());
        }
        clientResponse.setContent("Content-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);
    }
    htmlFile.close();
    _isDir = false;
    clientResponse.sendResponse();
}

/* WHEN AUTOINDEX IS ON, LIST ALL DIRECTORIES ON THE SCREEN */
std::string Client::generateDirectoryListing(std::string dirPath)
{
    std::string listing;
    listing += "<h3>";
    listing += "<ul>";
    for (const auto &entry : fs::directory_iterator("root/" + dirPath))
    {
        listing += "<li>";

        std::string fileName = entry.path().filename().string();
        std::string displayName = entry.path().stem().string(); // Remove extension
        // std::cou400 METHOD NOT ALLOWEDt << "Display Name = " << displayName << std::endl;

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
    Response clientResponse(m_socketFd, "302 FOUND");
    clientResponse.setContent("Location: " + getFileNameBody() + "\n\n");
    clientResponse.sendResponse();
    handleGetMethod();
}

/* DELETE */
void Client::handleDeleteMethod()
{
    std::string filePath = "root/" + getFileNameBody(); // Replace with your actual file path
    if (fs::exists(filePath))
    { // Check if the file exists
        try
        {
            fs::remove(filePath); // Remove the file
            std::cout << "File deleted successfully." << std::endl;
            Response goodResponse(m_socketFd, "204 No Content");
            goodResponse.sendResponse();
        }
        catch (const fs::filesystem_error &e)
        {
            std::cerr << "Error deleting the file: " << e.what() << std::endl;
            // setError(m_socketFd, "500 Internal Server Error");
            handleGetMethod();
        }
    }
    else
    {
        std::cout << "File does not exist." << std::endl;
        Response goodResponse(m_socketFd, "204 No Content");
        goodResponse.sendResponse();
    }
    handleGetMethod();
}
