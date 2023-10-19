#include "../../inc/main.hpp"

/* Create a respond to the client */

void Server::createResponse(Client* client) {
    std::cout << client->getMethod() << std::endl;

    // Read the content of the index.html file
    std::ifstream indexFile("docs/index.html");
    if (!indexFile.is_open()) {
        // If the file can't be opened, send an error response
        char response[] = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 13\n\nFile not found";
        send(client->getClientSocket(), response, strlen(response), 0);
    } else {
        // If the file is opened successfully, read its content and send it as the response
        std::string fileContent((std::istreambuf_iterator<char>(indexFile)), (std::istreambuf_iterator<char>()));
        indexFile.close();
        
        std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;
        send(client->getClientSocket(), response.c_str(), response.size(), 0);
    }
    
    printf("------------------Response sent-------------------\n");
}
