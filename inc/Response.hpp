#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "main.hpp"

class Client;

class Response
{
    public:
    // std::string response;
    //     Response();
    //     ~Response();

    void createResponse(Client* client);
    // void createErrorResponse(const std::string& errorMessage, Client *client);
    // void getMethod(Client* client);
    // bool isPathAndMethodAllowed(Client* client);
    // void postMethod(Client *client);
};

#endif