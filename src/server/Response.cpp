#include "../../inc/main.hpp"

/* Create a respond to the client */

void Server::createResponse(Client* client) {
    int method = client->getNbMethod();
    switch (method)
    {
        case 1:
            std::cout << "GET method" << std::endl;
            break;
        case 2:
            std::cout << "POST method" << std::endl;

            break;
        case 3:
            std::cout << "DELETE method" << std::endl;
            break;
        default:
            std::cout << "default" << std::endl;
    }

    /* GET*/

    // Read the content of the index.html file
    std::ifstream indexFile("index.html");
    if (!indexFile.is_open()) {
        // If the file can't be opened, send an error response
        char response[] = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 14\n\nFile not found";
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