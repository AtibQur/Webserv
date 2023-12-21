#include "../../inc/main.hpp"

namespace fs = std::filesystem;

Client::Client() : m_server(nullptr), _requestBuffer(""), _boundary("UNSET") {
    m_socketFd = -1;
}

Client::Client(Server &server, std::map<std::string, std::string> ErrorPages, std::map<std::string, Location> Locations) \
    : m_server(server), _boundary("UNSET") {
    _error_pages = ErrorPages;
    _location = Locations;
    _maxBodySize = server.getConf()->getMaxBodySize();

    m_socketFd = accept(server.getSockFd(), (struct sockaddr *)&m_client_address, &m_addrlen);
    if (m_socketFd == -1)
    {
        perror("client Accept() error");
        exit(EXIT_FAILURE);
    }
}

Client::~Client() {
    std::cout << "Client removed" << std::endl;
}

// Client::Client(Client const &copy) {
//     *this = copy;
// }

Client& Client::operator=(Client const &copy) {
    this->m_socketFd = copy.m_socketFd;
    this->_requestBuffer = copy._requestBuffer;
    this->_response = copy._response;
    return *this;
}

int Client::getNbMethod() { 
    if (_method == "GET") 
        return 1; 
    if (_method == "POST") 
        return 2; 
    if (_method == "DELETE") 
        return 3;
    return (0);
}

void Client::modifyEpoll(Socket *ptr, int events, int fd){
    struct epoll_event event;
    event.events = events;

    event.data.ptr = ptr;

    if (epoll_ctl(m_epoll, EPOLL_CTL_MOD, fd, &event) == -1) {
        perror("epoll_ctl mod out"); 
        exit(EXIT_FAILURE);
    }
    std::cout << "modified epoll" << std::endl;
}

void Client::receiveRequest() {
    readBuffer();
}

void Client::handleRequest(std::string request, ssize_t post) {
    try {
        parseRequest(request, post);
        isPathAndMethodAllowed();
    } catch (const std::exception& e) {
        std::cout << "this error: " << e.what() << std::endl;
        Response error(getSocketFd(), e.what());
        _response = error;
    }
}

void Client::readBuffer() {
    ssize_t post = 0;
    ssize_t i = 0;
    char buffer[1024] = {0};
    ssize_t bytes_read;
    std::string accumulatedRequestData;
    std::string hardcodedrequest;

    while (1)
    {
        bytes_read = read(getSocketFd(), buffer, sizeof(buffer));
        if (bytes_read < 0) {
            perror("Error reading from the client socket");
            close (getSocketFd());
            break ;
        } else if (bytes_read == 0){
            std::cout << "Connection closed by the client." << std::endl;
            close (getSocketFd());
            break ;
        } else if (i = 0) {
            if (buffer[0] == 'P' && buffer[1] == 'O' && buffer[2] == 'S' && buffer[3] == 'T' && buffer[4] == ' ')
                post = 1;
        }
        else
        {
            accumulatedRequestData.append(buffer, bytes_read); // append the request and break when it's complete
            if (bytes_read == 1024)
                continue ;
            if (isRequestComplete(accumulatedRequestData, post)) 
            {
                // modify epoll
                modifyEpoll(this, EPOLLOUT, getSocketFd());
                handleRequest(accumulatedRequestData, post);
                break ;
            }
        }
        i++;
    }
}

bool Client::isRequestComplete(std::string accumulatedRequestData, ssize_t post){
    ssize_t requestEnd;
    std::string boundary;
    ssize_t startOfBoundary = accumulatedRequestData.find("boundary=");
    if (post)
        requestEnd = accumulatedRequestData.find("\r\n\r\n");
    else
        requestEnd = accumulatedRequestData.find("\r\n\r\n");
    if (requestEnd == std::string::npos){
        std::cout << "the request is not complete" << std::endl;
        return false;
    }
    else {
        return true;
    }
}

bool Client::isPathAndMethodAllowed()
{
    Location clientLocation = m_server.getConf()->getLocation(getUri());

    if (!fs::exists("root" + getUri()))
        throw std::invalid_argument("404");
    if (clientLocation.getPath().empty())
    {
        throw std::invalid_argument("404");
    }
    if ("/root/" + access(getUri().c_str(), R_OK) == 0)
    {
        return true;
    }
    std::vector<std::string> methods = clientLocation.getMethods();
    if (methods.empty())
        throw std::invalid_argument("400");
    std::vector<std::string>::iterator it = methods.begin();
    for (it; it < methods.end(); it++)
    {
        if (getMethod() == *it) 
        {
            return true;
        }
    }
    throw std::invalid_argument("400");
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
                std::cout << "default method" << std::endl;
        }
    }
    else
    {
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

    std::string file;
    Location clientLocation = m_server.getConf()->getLocation(getUri());
    if (clientLocation.getPath() == getUri())
        file = "root" + clientLocation.getPath() + "/" + clientLocation.getIndex();
    else
        file = "root" + getUri();
    std::ifstream htmlFile(file);
    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));

    if (!htmlFile.is_open()) {
        clientResponse.setContent("14\n\nFile not found");
    } else {
        if (clientLocation.getAutoIndex()) {
            fileContent += generateDirectoryListing(clientLocation.getPath());
        }
        clientResponse.setContent("Content-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);
    }
    htmlFile.close();

    clientResponse.sendResponse();
}

/* WHEN AUTOINDEX IS ON, LIST ALL DIRECTORIES ON THE SCREEN */
std::string Client::generateDirectoryListing(std::string dirPath) {
    std::string listing;

    listing += "<ul>";
    for (const auto& entry : std::filesystem::directory_iterator("root/" + dirPath)) {
        listing += "<li>";

        std::string fileName = entry.path().filename().string();
        std::string displayName = entry.path().stem().string(); // Remove extension
        std::cout << "Display Name = " << displayName << std::endl;

        if (std::filesystem::is_directory(entry.path())) {
            listing += "[DIR] " + fileName;
                listing += generateDirectoryListing(dirPath + "/" + fileName);
        } else {
            listing += displayName;
        }
        listing += "</li>";
    }
    listing += "</ul>";

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
    std::string filePath = "root/" + getFileNameBody();  // Replace with your actual file path
    std::cout << "File path: " << filePath << std::endl;
    // Check if the file exists
    if (std::filesystem::exists(filePath)) {
        try {
            // Remove the file
            std::filesystem::remove(filePath);
            std::cout << "File deleted successfully." << std::endl;
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error deleting the file: " << e.what() << std::endl;
            handleGetMethod();
        }
    } else {
        std::cout << "File does not exist." << std::endl;
    }
    handleGetMethod();
}

void Client::createErrorResponse()
{
    std::string file;
    std::string response;

    file = m_server.getConf()->getErrorPage(_response.getCode());
    std::ifstream htmlFile(file);

    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));
    htmlFile.close();
    std::string array[4] {
        "docs/error_pages/400.html",
        "docs/error_pages/404.html",
        "docs/error_pages/405.html",
        "docs/error_pages/fourofour.html"
    };

     // 403 413 418 500 501 505
    for (int i = 0; i < 4; i++) {
        if (file == array[i]) {
            switch (i) {
                case 0:
                    _response.setResponseTemp("HTTP/1.1 400 Bad Request\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);
                    break;
                case 1:
                    _response.setResponseTemp("HTTP/1.1 404 Not Found\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);
                    break;
                case 2:
                    _response.setResponseTemp("HTTP/1.1 405 Method Not Allowed\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);
                    break;
                case 3:
                    _response.setResponseTemp("HTTP/1.1 404 Not Found\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);
                    break;
            }
        }
    }
    _response.sendResponse();
}

