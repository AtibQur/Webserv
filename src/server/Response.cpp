#include "../../inc/main.hpp"

/* Create a respond to the client */

void Server::createResponse(Client* client) {
    try {
        isPathAndMethodAllowed(client);
    } catch (const std::exception& e) {
        createErrorResponse(e.what(), client);
        return ;
    }
    int method = client->getNbMethod();
    switch (method)
    {
        case 1:
            getMethod(client);
            break;
        case 2:
            postMethod(client);
            break;
        case 3:
            break;
        default:
            std::cout << "default method" << std::endl;
    }
}

/* GET*/
void Server::getMethod(Client* client) {
    const char* file;
    std::string stringfile;
    Location clientLocation = _conf->getLocation(client->getUri());

    stringfile = "docs/" + clientLocation.getIndex();
    file = stringfile.c_str();

    std::ifstream htmlFile(file);

    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));

    htmlFile.close();
    if (file == "docs/error_pages/404.html") {
        std::string response = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;;
        send(client->getSocketFd(), response.c_str(), response.size(), 0);
    }


    std::ifstream indexFile("docs/index.html");
    if (!indexFile.is_open()) {
        char response[] = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 13\n\nFile not found";
        send(client->getSocketFd(), response, strlen(response), 0);
    } else {
        std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;
        send(client->getSocketFd(), response.c_str(), response.size(), 0);
    }
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
# define stdendl std::endl

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