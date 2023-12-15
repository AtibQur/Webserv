#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "main.hpp"

class Client;

class Response
{
private:
    std::string     _errorCode;
    int             _socketFd;
    std::string     _filePath;
    std::string     _head;
    std::string     _content_type;
    Config          *_conf;

public:
    Response();
    Response(int socketFd, std::string error);
    ~Response() {;};

    void    getMethod();
    void    createResponse(Client* client);
    bool    isPathAndMethodAllowed(Client* client);
    void    createErrorResponse(const std::string& errorMessage);

    // GETTERS
    std::string getError() { return _errorCode; };

    // SETTERS
    void    setConf(Config *conf) { _conf = conf; };
};

#endif