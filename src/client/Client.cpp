#include "Client.hpp"

namespace fs = std::filesystem;

Client::Client() : m_server(nullptr), _requestBuffer(""), _boundary("UNSET"), m_name(""), _isDir(false), m_cgiToServer(*this), m_serverToCgi(*this)
{
    m_socketFd = -1;
    _query = "";
    _path = "";
}

Client::Client(Server &server, std::map<std::string, std::string> ErrorPages, std::map<std::string, Location> Locations)
    : m_server(server), _boundary("UNSET"), m_cgiToServer(*this), m_serverToCgi(*this)
{
    _error_pages = ErrorPages;
    _location = Locations;
    _query = "";
    _isDir = false;
    _path = "";
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
    std::cout << "client operator construcotr called " << std::endl;
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

void Client::removeFromEpoll(int fd){
    modifyEpoll(nullptr, 0, fd);
    if (epoll_ctl(m_epoll, EPOLL_CTL_DEL, fd, NULL) == -1)
        perror ("remove epoll");
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
        checkPathAndMethod();

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
            std::cerr << "Error reading from client socket" << std::endl;
            throw(std::invalid_argument("400 Bad Request"));
            delete this;
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
            try {
                if (isRequestComplete(accumulatedRequestData, post))
                {
                    handleRequest(accumulatedRequestData, post);
                    break;
                }
            } catch (const std::exception &e) {
                setError(getSocketFd(), e.what());
                createErrorResponse();
            }
        }
        i++;
    }
}

bool Client::isRequestComplete(std::string accumulatedRequestData, ssize_t post)
{
    ssize_t requestEnd;

    requestEnd = accumulatedRequestData.find("\r\n\r\n");
    if (requestEnd == std::string::npos)
    {
        throw std::invalid_argument("400 Bad Request");
        return false;
    }
    else
    {
        return true;
    }
}

/* REQUEST CHECK IF THE URI IS IN LOCATIONS AND IF THE METHOD IS ALLOWED*/
bool Client::checkPathAndMethod()
{
    Location clientLocation = m_server.getConf()->getLocation(getUri());

    /* CGI */
    // if (getUri().find(".py") != std::string::npos)
    // {
    //     std::cout << "getMethod: " << getMethod() << std::endl;
    //     std::cerr << "serverToCgi to Epollout " << m_serverToCgi.m_pipeFd[WRITE] << std::endl;
    //     std::cerr << "cgiToServer to Epollin " << m_cgiToServer.m_pipeFd[READ] << std::endl;
    
    //     //TODO seperate depening on GET or POST
    //     addCGIProcessToEpoll(&m_serverToCgi, EPOLLOUT, m_serverToCgi.m_pipeFd[WRITE]); // add write end to pipeIn to epoll
    //     addCGIProcessToEpoll(&m_cgiToServer, EPOLLIN, m_cgiToServer.m_pipeFd[READ]); // add PipeOut to epoll

    //     //TODO check return value for right error throws
    //     // if (returnValue = 1) {
    //     //     throw (std::invalid_argument("500 Internal server error"));
    //     // }
    //     // if (returnValue == 2) {
    //     //     throw std::invalid_argument("404 Not Found");
    //     // }
    //     return true;
	// }
    /* CGI */

    modifyEpoll(this, EPOLLOUT, getSocketFd()); //? add Client to EPOLLOUT

    if (_method == "DELETE")
        return true;
    if (getUri() == "/teapot")
    {
        throw std::invalid_argument("418 I'm a teapot");
    }
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
            setError(getSocketFd(), "404");
            createErrorResponse();
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
    if (_response.getHeader().empty())
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
    if (!_response.getResponseMessage().empty()) //? Is CGI!
    {
        _response.setSocketFd(m_socketFd);
        _response.sendResponse();
        return;
    }
    Response clientResponse(m_socketFd, "200 OK");
    _response = clientResponse;

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
        _response.setContent("Content-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);
    }
    _isDir = false;
    htmlFile.close();
    _response.sendResponse();
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

    if (getFileNameBody().empty())
    {
        std::cout << "No file name" << std::endl;
        handleGetMethod();
    }
    Response clientResponse(m_socketFd, "302 FOUND");
    
    std::string filePath = "root/" + getFileNameBody();
    if (filePath == "root/") {
        std::cerr << "Error: Empty request" << std::endl;
        return ;
    }
    std::cout << "File path: " << filePath << std::endl;

    std::ifstream htmlFile(filePath);
    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));
    if (!htmlFile.is_open())
    {
        std::cerr << "Error opening file" << std::endl;

    }
    clientResponse.setContent("Location: " + getFileNameBody() + "\n\n");
    clientResponse.setContent("Content-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);
    clientResponse.sendResponse();
    htmlFile.close();
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
            Response goodResponse(m_socketFd, "202 Accepted");
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

/* CREATE ERROR RESPONSE */
void Client::createErrorResponse()
{
    std::string file;
    std::string response;

    file = m_server.getConf()->getErrorPage(_response.getCode());
    std::cout << "Error code: " << _response.getCode() << std::endl;
    std::ifstream htmlFile(file);

    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));
    htmlFile.close();

    _response.setErrorResponse("HTTP/1.1 " + _response.getCode() + "\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);

    _response.sendResponse();
}
