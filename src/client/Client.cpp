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
                handleRequest(accumulatedRequestData, buffer, post); 
                break ;
            }
        }
        i++;
    }
}

void Client::modifyEpoll(Socket *ptr, int events, int fd){
    struct epoll_event event;
    event.events = events;

    event.data.ptr = ptr;

    if (epoll_ctl(m_epoll, EPOLL_CTL_MOD, fd, &event) == -1) {
        perror("epoll_ctl mod out"); 
        exit(EXIT_FAILURE);
    }
    // std::cout << "modified epoll" << std::endl;
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

void Client::handleRequest(std::string request, char *buffer, ssize_t post) {
    try {
        parseRequest(request, buffer, post);
        isPathAndMethodAllowed();
    } catch (const std::exception& e) {
        Response error(getSocketFd(), e.what());
        error.setConf(m_server.getConf());
        _response = error;
    }
}

bool Client::isPathAndMethodAllowed()
{
    Location clientLocation = m_server.getConf()->getLocation(getUri());
    std::cout << "hi:" << getUri() << std::endl;
    std::cout << "Location: O" << clientLocation.getPath() << "0" << std::endl;
        
    if (!fs::exists("root" + getUri()))
        throw std::invalid_argument("404");
    if (clientLocation.getPath().empty())
    {
        throw std::invalid_argument("404");
    }
    if ("/root/" + access(getUri().c_str(), R_OK) == 0)
    {
        std::cout << getUri() << std::endl;
        return true;
    }
    std::vector<std::string> methods = clientLocation.getMethods();
    if (methods.empty())
        throw std::invalid_argument("400");
    std::vector<std::string>::iterator it = methods.begin();
    for (it; it < methods.end(); it++)
    {
        if (getMethod() == *it)
            return true;
    }
    throw std::invalid_argument("400");
}

void Client::sendResponse() {

    if (_response.getError().size() == 3){
        _response.createErrorResponse(_response.getError());
    }
    else {
        Location clientLocation = m_server.getConf()->getLocation(getUri());
        std::string file = "root" + clientLocation.getPath() + "/" + clientLocation.getIndex();
        Response clientResponse(getSocketFd(), file);

        clientResponse.setConf(m_server.getConf());
        _response = clientResponse;

        _response.createResponse(this);
    }
    modifyEpoll(this, EPOLLIN, getSocketFd());
    Response res;
    _response = res;
}
