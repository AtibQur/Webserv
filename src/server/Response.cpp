#include "../../inc/main.hpp"

/* Create a respond to the client */

void Server::createResponse(Client* client) {
    //GET //POST //DELETE
    // home page and upload files page everything else is 404
    char response[] = "HTTP/1.1 200\nContent-Type: text/plain\nContent-Length: 37\n\nHello mi brothas\nHope all is well :)\n";
    send(client->getClientSocket(), response, strlen(response), 0);
    printf("\n------------------response sent-------------------\n");
}
