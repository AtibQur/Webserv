#include "../../inc/main.hpp"

Client::Client(){}

Client::Client(int client_socket){
    clientSocket = client_socket;
}

Client::~Client() {
    std::cout << "Client removed" << std::endl;
}

Client::Client(Client const &copy) {
    *this = copy;
}

Client& Client::operator=(Client const &copy) {
    *this = copy;
    return *this;
}

int Client::handleRequest(char *buffer) {
    try {
        parseRequest(buffer);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        createErrorResponse(e.what());
    }
    return (0);
}

void Client::createErrorResponse(const std::string& errorMessage) {
    std::cout << "respone: " << std::endl;
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