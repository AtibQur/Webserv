#include "../../inc/main.hpp"

/* Create a respond to the client */
Response::Response() : _socketFd(0), _filePath(""), _errorCode("") {}

Response::Response(int SocketFd, std::string error) : _socketFd(SocketFd), _errorCode(error) {
    _filePath = error;
}

void Response::createResponse(Client* client) {
    // try {
    //     isPathAndMethodAllowed(client);
    // } catch (const std::exception& e) {
    //     createErrorResponse(e.what(), client);
    //     return ;
    // }
    int method = client->getNbMethod();
    switch (method)
    {
        case 1:
            getMethod(client);
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
void Response::getMethod(Client *client) {
    const char* file;
    std::string response;

    Location location = _conf->getLocation(client->getUri());
    file = _filePath.c_str();
    std::ifstream htmlFile(file);
    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));

    if (!htmlFile.is_open()) {
        response = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 14\n\nFile not found";
    } else {
        if (location.getAutoIndex()) {
            fileContent += generateDirectoryListing(location.getPath());
        }
        response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;
    }
    htmlFile.close();

    send(_socketFd, response.c_str(), response.size(), 0);
    printf("------------------Response sent-------------------\n");
}

/* WHEN AUTOINDEX IS ON, LIST ALL DIRECTORIES ON THE SCREEN*/
std::string Response::generateDirectoryListing(std::string dirPath) {
    std::string listing;

    listing += "<ul>";
    for (const auto& entry : std::filesystem::directory_iterator("root/" + dirPath)) {
        std::cout << entry.path() << std::endl;

        if (std::filesystem::is_directory(entry.path())) {
            listing += "<li>[DIR] " + entry.path().filename().string();
            listing += generateDirectoryListing(dirPath + "/" + entry.path().filename().string());
        } else {
            listing += "<li>" + entry.path().filename().string() + "</li>";
        }
    }
    listing += "</ul>";

    return listing;
}

/* RETURN ERROR WHEN REQUEST IS WRONG*/
void Response::createErrorResponse(const std::string& errorMessage)
{
    std::string file;
    std::string response;

    std::cout << "error: " << errorMessage << "\n";

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
    send(_socketFd, response.c_str(), response.size(), 0);
    printf("------------------Error Response sent-------------------\n");
}