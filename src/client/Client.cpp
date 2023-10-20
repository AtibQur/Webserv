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
    std::string httpRequest = buffer;
    size_t nl = httpRequest.find("\r\n");
    if (nl == std::string::npos)
        perror("400: invalid request: can't find the request line");
    std::string requestLine = httpRequest.substr(0, nl);
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
    size_t startHeaderSection = httpRequest.find("\n");
    if (startHeaderSection == std::string::npos)
        perror("400: can't find the headersection line");
    std::string headerSection = httpRequest.substr(startHeaderSection + 1);

    size_t pos = 0;
    while (pos < headerSection.size()){
        size_t lineEnd = headerSection.find("\n", pos);
        if (lineEnd == std::string::npos)
            perror("400: can't find the end of the line");
        std::string line = headerSection.substr(pos, lineEnd - pos);

        size_t seperatorPos = line.find(":");
        if (seperatorPos != std::string::npos){
            std::string key = line.substr(0, seperatorPos);
            std::string value = line.substr(seperatorPos + 2);
            headerMap[key] = value;
        } else {
            perror("400: can't find the seperator");
        }
        pos = lineEnd + 1;
    }

    // request body
    std::string requestBody;
    if (headerMap.find("Content-Type") != headerMap.end()){
        requestBody = headerMap["Content-Type"];
        if (requestBody.find("boundary=") != std::string::npos){
            this->_boundary = requestBody.substr(requestBody.find('=') + 1);
        }
    }

    // print request line
    std::cout << _method << " " << _uri << " " << _protocol << std::endl;
    // print map
    std::map<std::string, std::string>::iterator it = headerMap.begin();
    while (it != headerMap.end())
    {
        std::cout << "-> " << it->first << " -> " << it->second << std::endl;
        ++it;
    }
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