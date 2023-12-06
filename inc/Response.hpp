#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "main.hpp"

class Client;

class Response
{
private:
    std::string response;
    int         _socketFd;
    std::string _filePath;
    Config     *_conf;

public:
    Response();
    Response(int socketFd, std::string filePath);
    ~Response() {;};

    void getMethod();
    void createResponse(Client* client);
    bool isPathAndMethodAllowed(Client* client);
    void createErrorResponse(const std::string& errorMessage, Client *client);

    void setConf(Config *conf) { _conf = conf; };
};

#endif