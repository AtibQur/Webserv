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

/* Parse the client request */

void Client::parseRequest(char* buffer) {
    std::string httpRequest;
    std::istringstream ss(buffer);
    getline(ss, httpRequest);

    // std::size_t contentDispositionPos = httpRequest.find("Content-Disposition");
    // if (contentDispositionPos != std::string::npos)
    //     std::cout << "FOUND!" << std::endl;

    std::string requestLine = httpRequest.substr(0);
    size_t methodEnd = requestLine.find(' ');
    if (methodEnd == std::string::npos)
        perror("400: can't find the end of the method");
    size_t uriEnd = requestLine.find(' ', methodEnd + 1);
    if (uriEnd == std::string::npos)
        perror("400: can't find the end of the uri");

    // parse request line
    this->_method = requestLine.substr(0, methodEnd);
    this->_uri = requestLine.substr(methodEnd + 1, uriEnd - methodEnd - 1);
    this->_protocol = requestLine.substr(uriEnd + 1);

    // parse headers
    size_t pos = 0;
    while (getline(ss, httpRequest) && httpRequest[0] != '\r')
    {
        size_t index = httpRequest.find(':', 0);
        if (index == std::string::npos){
            perror("400: can't find the seperator");
            break ;
        }
        std::string key = httpRequest.substr(0, index);
        std::string value = httpRequest.substr(index + 2);
        headerMap[key] = value;
    }

    // // request body
    // std::string requestBody;
    // if (headerMap.find("Content-Type") != headerMap.end()){
    //     requestBody = headerMap["Content-Type"];
    //     if (requestBody.find("boundary=") != std::string::npos){
    //         this->_boundary = requestBody.substr(requestBody.find('=') + 1);
    //     }
    // }

    // // print request line
    std::cout << "request line: " << _method << " " << _uri << " " << _protocol << std::endl;
    // print map
    std::map<std::string, std::string>::iterator it = headerMap.begin();
    while (it != headerMap.end())
    {
        std::cout << "-> " << it->first << " -> " << it->second << std::endl;
        ++it;
    }
    // std::cout << "b --> " << this->_boundary << std::endl;
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