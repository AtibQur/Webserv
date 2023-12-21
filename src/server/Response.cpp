#include "../../inc/main.hpp"

/* Create a respond to the client */
Response::Response() : _socketFd(0), _filePath(""), _code("") {}

Response::Response(int SocketFd, std::string error) : _socketFd(SocketFd), _code(error) {
    _filePath = error;
}

void Response::createResponse(Client* client) {
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
            deleteMethod(client);
            break;
        default:
            std::cout << "default method" << std::endl;
    }
}

/* DELETE*/
void Response::deleteMethod(Client *client) {
    std::string filePath = "root/" + client->getFileNameBody();  // Replace with your actual file path
    std::cout << "File path: " << filePath << std::endl;
    // Check if the file exists
    if (std::filesystem::exists(filePath)) {
        try {
            // Remove the file
            std::filesystem::remove(filePath);
            std::cout << "File deleted successfully." << std::endl;
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error deleting the file: " << e.what() << std::endl;
            getMethod(client);
        }
    } else {
        std::cout << "File does not exist." << std::endl;
    }
    getMethod(client);
}

/* POST*/
void Response::postMethod(Client *client) {
    std::string response = "HTTP/1.1 302 FOUND\nLocation: " + client->getFileNameBody() + "\n\n";
    send(_socketFd, response.c_str(), response.size(), 0);
    getMethod(client);
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
        response = "HTTP/1.1 404 Not Found\nContent-Length: 14\n\nFile not found";
    } else {
        if (location.getAutoIndex()) {
            fileContent += generateDirectoryListing(location.getPath());
        }
        response = "HTTP/1.1 200 OK\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;
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
        listing += "<li>";

        std::string fileName = entry.path().filename().string();
        std::string displayName = entry.path().stem().string(); // Remove extension
        std::cout << "Display Name = " << displayName << std::endl;

        if (std::filesystem::is_directory(entry.path())) {
            listing += "[DIR] " + fileName;
                listing += generateDirectoryListing(dirPath + "/" + fileName);
        } else {
            listing += displayName;
        }
        listing += "</li>";
    }
    listing += "</ul>";

    return listing;
}



/* RETURN ERROR WHEN REQUEST IS WRONG*/
void Response::createErrorResponse(const std::string& errorMessage)
{
    std::string file;
    std::string response;

    file = _conf->getErrorPage(errorMessage);
    std::ifstream htmlFile(file);

    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));
    htmlFile.close();
    std::string array[4] {
        "docs/error_pages/400.html",
        "docs/error_pages/404.html",
        "docs/error_pages/405.html",
        "docs/error_pages/fourofour.html"
    };

     // 403 413 418 500 501 505
     // HTTP/1.1
     // HEADER
     // content-length:
    for (int i = 0; i < 4; i++) {
        if (file == array[i]) {
            switch (i) {
                case 0:
                    response = "HTTP/1.1 400 Bad Request\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;
                    break;
                case 1:
                    response = "HTTP/1.1 404 Not Found\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;
                    break;
                case 2:
                    response = "HTTP/1.1 405 Method Not Allowed\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;
                    break;
                case 3:
                    response = "HTTP/1.1 404 Not Found\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent;
                    break;
            }
        }
    }
    send(_socketFd, response.c_str(), response.size(), 0);
    printf("------------------Error Response sent-------------------\n");
}

