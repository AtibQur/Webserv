#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "main.hpp"

class Client;

class Response
{
private:
    std::string     m_response;
    std::string     _code;
    int             _socketFd;
    Config          *_conf;

public:
    Response();
    Response(int socketFd, std::string error);
    ~Response() {;};

    std::string getCode() { return _code; };
    void        setResponse(std::string message);
    void        setContent(std::string content);
    void        sendResponse();
    void        setConf(Config *conf) { _conf = conf; }
    void        setErrorResponse(std::string message);
};

#endif