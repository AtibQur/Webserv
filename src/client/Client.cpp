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

int Client::getNbMethod() { 
    if (_method == "GET") 
        return 1; 
    if (_method == "POST") 
        return 2; 
    if (_method == "DELETE") 
        return 3;
    return (0);
}

void Client::readBuffer(Server* server) {

    char buffer[1024] = {0};
    ssize_t bytes_read;
    std::string accumulatedRequestData;
    std::string hardcodedrequest;

    std::cout << "request: " << buffer << std::endl;
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
        }
        else
        {
            _requestBuffer.append(buffer, bytes_read); // append the request and break when it's complete
            if (isRequestComplete(_requestBuffer)) 
            {
                // handleRequest(server, _requestBuffer); // parse the request with this client
                std::cout<< "complete" << std::endl;
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

void Client::handleRequest(Server* server, std::string request) {
    try {
        parseRequest(request);
    } catch (const std::exception& e) {
        server->createErrorResponse(e.what(), this);
        return ;
    }
    server->createResponse(this);
}