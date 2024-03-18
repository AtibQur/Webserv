#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "main.hpp"

class Client;

class Response
{
private:
    std::string     m_response;
    int             _socketFd;
    std::string     m_header;
    std::string     _code;
    Config          *_conf;

public:
    // std::string     m_response;
    Response();
    Response(int socketFd, std::string error);
    ~Response() {;};

    /* GETTERS */
    std::string getCode() { return _code; };
    std::string getHeader() { return m_header; }

    /* SETTERS */
    void        setHeader(std::string header) { m_header = header; };
    void        setResponse(std::string message);
    void        setContent(std::string content);
    void        setConf(Config *conf) { _conf = conf; }
    void        setErrorResponse(std::string message);
    void        setSocketFd(int socketFd) { _socketFd = socketFd; };
    void        setServerName(std::vector<std::string> server_names);

    void        sendResponse();
    std::string getResponseMessage() { return m_response; }
};

#endif