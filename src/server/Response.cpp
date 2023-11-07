#include "../../inc/main.hpp"

/* Create a respond to the client */

void Server::createResponse(Client* client) {
    try {
        isPathAndMethodAllowed(client); // check if the path and method are in the config file
    } catch (const std::exception& e) {
        createErrorResponse(e.what(), client);
        return ;
    }

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
    const char* file;
    Location clientLocation = _conf->getLocation(client->getUri());

    std::cout << "INDEX: " << clientLocation.getIndex() << std::endl;
    
    // good? - return index

    
    if (client->getUri() == "/")
        file = "docs/index.html";
    else if (client->getUri() == "/upload")
        file = "docs/upload.html";
    else 
        file = "docs/error_pages/404.html";
    std::ifstream htmlFile(file);

    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));
    htmlFile.close();
    if (file == "docs/error_pages/404.html") {
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

bool Server::isPathAndMethodAllowed(Client *client)
{
    Location clientLocation = _conf->getLocation(client->getUri());
    if (clientLocation.getPath().empty()){
        throw std::invalid_argument("405");
    }
    std::vector<std::string> methods = clientLocation.getMethods();
    if (methods.empty())
        throw std::invalid_argument("400");
    std::vector<std::string>::iterator it = methods.begin();
    for (it; it < methods.end(); it++){
         std::cout << *it << std::endl;
        if (client->getMethod() == *it)
            return true;
    }
    throw std::invalid_argument("400");
}

void Server::createErrorResponse(const std::string& errorMessage, Client *client){
    std::cout << "HTTP/1.1 " << errorMessage << std::endl;

    const char* file;

    file = "docs/error_pages/404.html";
    std::ifstream htmlFile(file);

    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));
    htmlFile.close();
    if (file == "docs/error_pages/404.html") {
        // 404 page
        std::string response = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;;
        send(client->getClientSocket(), response.c_str(), response.size(), 0);
    }
}