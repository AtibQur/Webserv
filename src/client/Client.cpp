#include "../../inc/main.hpp"

Client::Client() : _socketFd(-1), _requestBuffer("") {
}

Client::Client(int newSocketFd) : _socketFd(newSocketFd) {
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

int Client::handleRequest(std::string request, char *buffer) {
    try {
        parseRequest(request, buffer);
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
        }
        else
        {
            accumulatedRequestData.append(buffer, bytes_read); // append the request and break when it's complete
            if (isRequestComplete(accumulatedRequestData)) 
            {
                handleRequest(accumulatedRequestData, buffer); // parse the request with this client
                // close or delete client?
                break ;
            }
        }
    }
}

bool Client::isRequestComplete(std::string accumulatedRequestData){
    ssize_t requestEnd;
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
