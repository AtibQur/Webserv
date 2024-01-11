#include "main.hpp"

/* Create a respond to the client */
Response::Response() : m_response(""), _socketFd(0), _code("") {}

// error
Response::Response(int SocketFd, std::string newCode) : _socketFd(SocketFd), _code(newCode) {
    setResponse(_code);
}

void    Response::setResponse(std::string message) {
    m_response = "HTTP/1.1 " + message + "\n";
}

void    Response::setContent(std::string content) {  
    m_response += content;
}

void    Response::setErrorResponse(std::string message)
{
    m_response = message;
}

void Response::sendResponse() {
    send(_socketFd, m_response.c_str(), m_response.size(), 0);
    printf("------------------Response sent-------------------\n");
}
