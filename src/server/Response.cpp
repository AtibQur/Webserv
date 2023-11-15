#include "../../inc/main.hpp"

/* Create a respond to the client */

void Server::createResponse(Client* client) {
    // try {
    //     isPathAndMethodAllowed(client); // check if the path and method are in the config file
    // } catch (const std::exception& e) {
    //     createErrorResponse(e.what(), client);
    //     return ;
    // }
    getMethod(client);

}
    /* GET*/
void Server::getMethod(Client* client) {
    const char* file;
    Location clientLocation = _conf->getLocation(client->getUri());

    file = "docs/index.html";
    std::ifstream htmlFile(file);

    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));
    htmlFile.close();

    std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;
    send(client->getSocketFd(), response.c_str(), response.size(), 0);

    printf("------------------Response sent-------------------\n");
}

bool Server::isPathAndMethodAllowed(Client *client)
{
    Location clientLocation = _conf->getLocation(client->getUri());
    if (clientLocation.getPath().empty())
    {
        throw std::invalid_argument("404");
    }
    std::vector<std::string> methods = clientLocation.getMethods();
    if (methods.empty())
        throw std::invalid_argument("400");
    std::vector<std::string>::iterator it = methods.begin();
    for (it; it < methods.end(); it++)
    {
        if (client->getMethod() == *it)
            return true;
    }
    throw std::invalid_argument("400");
}


void Server::createErrorResponse(const std::string& errorMessage, Client *client)
{
    std::string file;
    std::string response;

    file = _conf->getErrorPage(errorMessage);
    std::ifstream htmlFile(file);

    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));
    htmlFile.close();
    if (file == "docs/error_pages/400.html") // 400
    {
        response = "HTTP/1.1 400 Bad Request\nContent-Type: text/html\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;
    }
    if (file == "docs/error_pages/404.html") // 404
    {
        response = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;
    }
    if (file == "docs/error_pages/405.html") // 405
    {
        response = "HTTP/1.1 405 Method Not Allowed\nContent-Type: text/html\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;
    }
    send(client->getSocketFd(), response.c_str(), response.size(), 0);
}