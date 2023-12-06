#include "../../inc/main.hpp"

/* Create a respond to the client */
Response::Response() : _socketFd(0), _filePath("") {}

Response::Response(int SocketFd, std::string filePath) : _socketFd(SocketFd), _filePath(filePath) {

}

void Response::createResponse(Client* client) {
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
            getMethod();
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            std::cout << "default method" << std::endl;
    }
}

/* GET*/
void Response::getMethod() {
    const char* file;
    std::string response;

    file = _filePath.c_str();

    std::ifstream htmlFile(file);
    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));

    if (!htmlFile.is_open()) {
        response = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 13\n\nFile not found";
    } else {
        response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;
    }
    htmlFile.close();

    send(_socketFd, response.c_str(), response.size(), 0);
    printf("------------------Response sent-------------------\n");
}

bool Response::isPathAndMethodAllowed(Client *client)
{
    Location clientLocation = _conf->getLocation(client->getUri());
    if (clientLocation.getPath().empty())
    {
        throw std::invalid_argument("404");
    }
    if ("/root/" + access(client->getUri().c_str(), R_OK) == 0)
    {
        std::cout << client->getUri() << std::endl;
        return true;
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

void Response::createErrorResponse(const std::string& errorMessage, Client *client)
{
    std::string file;
    std::string response;

    std::cout << errorMessage << "\n";

    file = _conf->getErrorPage(errorMessage);
    std::cout << file << std::endl;
    std::ifstream htmlFile(file);

    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));
    htmlFile.close();
    std::string array[4] {
        "docs/error_pages/400.html",
        "docs/error_pages/404.html",
        "docs/error_pages/405.html",
        "docs/error_pages/fourofour.html"
    };

    for (int i = 0; i < 4; i++) {
        if (file == array[i]) {
            switch (i) {
                case 0:
                    response = "HTTP/1.1 400 Bad Request\nContent-Type: text/html\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;
                    break;
                case 1:
                    response = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;
                    break;
                case 2:
                    response = "HTTP/1.1 405 Method Not Allowed\nContent-Type: text/html\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;
                    break;
                case 3:
                    response = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;
                    break;
            }
        }
    }
    std::cout << "!response: " << response << std::endl;
    send(_socketFd, response.c_str(), response.size(), 0);
    printf("------------------Error Response sent-------------------\n");
}