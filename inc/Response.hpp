#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "main.hpp"

class Client;

class Response
{
private:
    std::string     _code;
    int             _socketFd;
    std::string     _filePath;
    std::string     _head;
    std::string     _content_type;
    Config          *_conf;

public:
    Response();
    Response(int socketFd, std::string error);
    ~Response() {;};

    void    deleteMethod(Client *client);
    void    getMethod(Client *client);
    void    postMethod(Client *client);
    void    createResponse(Client* client);
    bool    isPathAndMethodAllowed(Client* client);
    void    createErrorResponse(const std::string& errorMessage);
    std::string generateDirectoryListing(std::string dirPath);

    // GETTERS
    std::string getError() { return _code; };

    // SETTERS
    void    setConf(Config *conf) { _conf = conf; };
};

#endif