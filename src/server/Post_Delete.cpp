#include "../../inc/main.hpp"

void Server::postMethod(Client *client) {

    std::string response = "HTTP/1.1 302 Found\nLocation: ../../root/" + client->getFileNameBody();
    send(client->getSocketFd(), response.c_str(), response.size(), 0);
    std::cout << response << std::endl;
    printf("------------------Response sent-------------------\n");
}