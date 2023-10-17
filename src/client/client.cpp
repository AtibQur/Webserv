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

void    Client::saveClientRequest(int client_socket) {
    char buffer[30000] = {0};
    long value_read = read(client_socket, buffer, 30000);
    if (buffer < 0){
        std::cerr << "Error reading from the client socket" << std::endl;
    }

    Client* client = new Client(client_socket);
    parseRequest(buffer, client);
    Client::clients.push_back(client); // add client to the clients list
}

void Client::parseRequest(char* buffer, Client* client) {
    std::string httpRequest = buffer;
    // std::cout << httpRequest << std::endl;
    std::string requestLine = httpRequest.substr(0, httpRequest.find("\r\n"));
    size_t methodEnd = requestLine.find(' ');
    size_t uriEnd = requestLine.find(' ', methodEnd + 1);
    
    // parse request line
    client->method = requestLine.substr(0, methodEnd);
    client->uri = requestLine.substr(methodEnd + 1, uriEnd - methodEnd - 1);
    client->protocol = requestLine.substr(uriEnd + 1);

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

    // request body
    std::string requestBody;
    if (headerMap.find("Content-Length") != headerMap.end()){
        std::cout << "There is a request body" << std::endl;
    }
}