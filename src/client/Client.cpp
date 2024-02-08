#include "../../inc/main.hpp"

namespace fs = std::filesystem;

Client::Client() : m_server(nullptr), _requestBuffer(""), _boundary("UNSET"), m_name(""), _isDir(false)
{
    m_socketFd = -1;
}

Client::Client(Server &server, std::map<std::string, std::string> ErrorPages, std::map<std::string, Location> Locations)
    : m_server(server), _boundary("UNSET")
{
    _error_pages = ErrorPages;
    _location = Locations;
    _maxBodySize = server.getConf()->getMaxBodySize();
    _file_if_dir = server.getConf()->getFileIfDir();

    m_socketFd = accept(server.getSockFd(), (struct sockaddr *)&m_client_address, &m_addrlen);
    if (m_socketFd == -1)
    {
        setError(m_socketFd, "500 Internal Server Error");
    }
}

Client::~Client()
{
    std::cout << "Client removed" << std::endl;
}

// Client::Client(Client const &copy) {
//     *this = copy;
// }

Client &Client::operator=(Client const &copy)
{
    this->m_socketFd = copy.m_socketFd;
    this->_requestBuffer = copy._requestBuffer;
    this->_response = copy._response;
    return *this;
}

int Client::getNbMethod()
{
    if (_method == "GET")
        return 1;
    if (_method == "POST")
        return 2;
    if (_method == "DELETE")
        return 3;
    return (0);
}

void Client::modifyEpoll(Socket *ptr, int events, int fd)
{
    struct epoll_event event;
    event.events = events;

    event.data.ptr = ptr;
    if (epoll_ctl(m_epoll, EPOLL_CTL_MOD, fd, &event) == -1)
    {
        std::cerr << "Error modifying epoll" << std::endl;
        setError(m_socketFd, "500 Internal Server Error");
    }
}

void Client::receiveRequest()
{
    try
    {
        readBuffer();
    }
    catch (const std::exception &e)
    {
        setError(getSocketFd(), e.what());
    }
}

void Client::handleRequest(std::string request, ssize_t post)
{
    try
    {
        parseRequest(request, post);
        isPathAndMethodAllowed();
    }
    catch (const std::exception &e)
    {
        setError(getSocketFd(), e.what());
    }
}

void Client::readBuffer()
{
    ssize_t post = 0;
    ssize_t i = 0;
    char buffer[1024] = {0};
    ssize_t bytes_read;
    std::string accumulatedRequestData;
    std::string hardcodedrequest;

    while (1)
    {
        bytes_read = read(getSocketFd(), buffer, sizeof(buffer));
        if (bytes_read < 0)
        {
            std::cerr << "Error reading form client socket" << std::endl;
            throw(std::invalid_argument("400 Bad Request"));
            close(getSocketFd());
            break;
        }
        else if (bytes_read == 0)
        {
            std::cout << "Connection closed by the client." << std::endl;
            close(getSocketFd());
            break;
        }
        else if (i = 0)
        {
            if (buffer[0] == 'P' && buffer[1] == 'O' && buffer[2] == 'S' && buffer[3] == 'T' && buffer[4] == ' ')
                post = 1;
        }
        else
        {
            accumulatedRequestData.append(buffer, bytes_read);
            if (bytes_read == 1024)
                continue;
            if (isRequestComplete(accumulatedRequestData, post))
            {
                modifyEpoll(this, EPOLLOUT, getSocketFd());
                handleRequest(accumulatedRequestData, post);
                break;
            }
        }
        i++;
    }
}

bool Client::isRequestComplete(std::string accumulatedRequestData, ssize_t post)
{
    ssize_t requestEnd;
    std::string boundary;
    ssize_t startOfBoundary = accumulatedRequestData.find("boundary=");
    if (post)
        requestEnd = accumulatedRequestData.find("\r\n\r\n");
    else
        requestEnd = accumulatedRequestData.find("\r\n\r\n");
    if (requestEnd == std::string::npos)
    {
        std::cout << "the request is not complete" << std::endl;
        return false;
    }
    else
    {
        return true;
    }
}

bool Client::isPathAndMethodAllowed()
{
    Location clientLocation = m_server.getConf()->getLocation(getUri());

    if (getUri() == "/teapot")
    {
        throw std::invalid_argument("418 I'm a teapot");
    }
    if (!fs::exists("root" + getUri())) {
        std::cout << "1" << std::endl;
        throw std::invalid_argument("404 Not Found");
    }
    if (fs::is_regular_file("root" + getUri()))
        return true;
    std::cout << clientLocation.getPath() << std::endl;
    std::cout << getUri() << std::endl;
    if (fs::is_directory("root" + getUri()) && clientLocation.getPath() != getUri())
    {
        std::cout << "2" << std::endl;
        _isDir = true;
        // throw std::invalid_argument("404 Not Found"); // Set a default file to answer if the request is a directory
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
        std::cout << "hi" << std::endl;
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
    if (_isDir) {
        std::cout << "is dir" << std::endl;
        filePath = "root/" + _file_if_dir;
    }
    else if (clientLocation.getPath() == getUri())
        filePath = "root" + clientLocation.getPath() + "/" + clientLocation.getIndex();
    else
        filePath = "root" + getUri();
    std::cout << "File path: " << filePath << std::endl;
    std::ifstream htmlFile(filePath);
    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));

    if (!htmlFile.is_open())
    {
        clientResponse.setContent("14\n\nFile not found");
    }
    else
    {
        if (clientLocation.getAutoIndex())
        {
            fileContent += generateDirectoryListing(clientLocation.getPath());
        }
        clientResponse.setContent("Content-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);
    }
    htmlFile.close();
    _isDir = false;
    std::cout << "response " << std::endl;
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
    std::cout << "File path: " << filePath << std::endl;
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

void Client::setError(int socket, std::string message)
{
    Response errorResponse(socket, message);
    _response = errorResponse;
}

void Client::createErrorResponse()
{
    std::string file;
    std::string response;

    file = m_server.getConf()->getErrorPage(_response.getHeader());
    std::cout << "Error code: " << _response.getHeader() << std::endl;
    std::ifstream htmlFile(file);

    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));
    htmlFile.close();

    _response.setErrorResponse("HTTP/1.1 " + _response.getCode() + "\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);

    _response.sendResponse();
}
