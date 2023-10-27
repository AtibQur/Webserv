#include "../../inc/main.hpp"

/* Create a respond to the client */

void Server::createResponse(Client* client) {
    int method = client->getNbMethod();
    switch (method)
    {
        case 1:
            std::cout << "GET method" << std::endl;
            getMethod(client);
            break;
        case 2:
            postMethod();
            break;
        case 3:
            std::cout << "DELETE method" << std::endl;
            break;
        default:
            std::cout << "default" << std::endl;
    }
}
    /* GET*/
void Server::getMethod(Client* client) {
    // if index / 404 /upload open that page
    const char* file;
    if (client->getUri() == "/")
        file = "docs/index.html";
    else if (client->getUri() == "/upload")
        file = "docs/upload.html";
    else 
        file = "docs/404.html";
    std::ifstream htmlFile(file);

    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));
    htmlFile.close();
    if (file == "docs/404.html") {
        // 404 page
        std::string response = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;;
        send(client->getClientSocket(), response.c_str(), response.size(), 0);
    }
    // std::cout << client->getMethod() << std::endl;

    // Read the content of the index.html file
    std::ifstream indexFile("docs/index.html");
    if (!indexFile.is_open()) {
        // If the file can't be opened, send an error response
        char response[] = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 13\n\nFile not found";
        send(client->getClientSocket(), response, strlen(response), 0);
    } else {
        // Correct page
        std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;
        send(client->getClientSocket(), response.c_str(), response.size(), 0);
    }
    printf("------------------Response sent-------------------\n");
}

// response zin eindigt met /r/n
// hele response eidigt met /r/n/r/n
// content length bepaalt of the body compleet is (als er een body is) 