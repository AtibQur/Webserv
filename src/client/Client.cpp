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

// void    Client::saveClientRequest(char* buffer, int client_socket) {
    // char buffer[30000] = {0};
    // long value_read = read(client_socket, buffer, 30000);
    // if (buffer < 0){
    //     perror("Error reading from the client socket");
    // }
    // Client* client = new Client(client_socket);
    // parseRequest(buffer, client);
    // Client::clients.push_back(client); // add client to the clients list
// }


/* Parse the client request */

void Client::parseRequest(char* buffer) {
    std::string httpRequest = buffer;
    std::string requestLine = httpRequest.substr(0, httpRequest.find("\r\n"));
    size_t methodEnd = requestLine.find(' ');
    size_t uriEnd = requestLine.find(' ', methodEnd + 1);
    
    // parse request line
    this->_method = requestLine.substr(0, methodEnd);
    this->_uri = requestLine.substr(methodEnd + 1, uriEnd - methodEnd - 1);
    this->_protocol = requestLine.substr(uriEnd + 1);

    // parse headers
    size_t startHeaderSection = httpRequest.find("\n");
    std::string headerSection = httpRequest.substr(startHeaderSection + 1);

    size_t pos = 0;
    while (pos < headerSection.size()){
        size_t lineEnd = headerSection.find("\n", pos);
        std::string line = headerSection.substr(pos, lineEnd - pos);

        size_t seperatorPos = line.find(":");
        if (seperatorPos != std::string::npos){
            std::string key = line.substr(0, seperatorPos);
            std::string value = line.substr(seperatorPos + 2);
            headerMap[key] = value;
        }
        pos = lineEnd + 1;
    }

    // print request line
    std::cout << _method << " " << _uri << " " << _protocol << std::endl;
    // print map
    std::map<std::string, std::string>::iterator it = headerMap.begin();
    while (it != headerMap.end())
    {
        std::cout << "-> " << it->first << "-> " << it->second << std::endl;
        ++it;
    }

    // request body
    std::string requestBody;
    if (headerMap.find("Content-Length") != headerMap.end()){
        std::cout << "There is a request body" << std::endl;
    }
}