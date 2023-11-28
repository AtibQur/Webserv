#include "../../inc/main.hpp"

Client::Client() : _socketFd(-1), _requestBuffer(""), _boundary("UNSET") {
}

Client::Client(int newSocketFd, std::map<std::string, std::string> ErrorPages, std::map<std::string, Location> Locations ) : _socketFd(newSocketFd), _boundary("UNSET") {
    _error_pages = ErrorPages;
    _location = Locations;
}

Client::~Client() {
    std::cout << "Client removed" << std::endl;
}

Client::Client(Client const &copy) {
    *this = copy;
}

Client& Client::operator=(Client const &copy) {
    this->_socketFd = copy._socketFd;
    this->_requestBuffer = copy._requestBuffer;
    return *this;
}

int Client::handleRequest(std::string request, char *buffer, ssize_t post) {
    try {
        parseRequest(request, buffer, post);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        createErrorResponse(e.what());
    }
    return (0);
}

void Client::createErrorResponse(const std::string& errorMessage) {
    std::cout << "response: " << std::endl;
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
                handleRequest(accumulatedRequestData, buffer, post); // parse the request with this client
                // close or delete client?
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
        std::cout << "the request is complete" << std::endl;
        return true;
    }
}
