#include "main.hpp"

/* Create a respond to the client */
Response::Response() : m_response(""), _socketFd(0), _code("") {}

// error
Response::Response(int SocketFd, std::string header) : _socketFd(SocketFd), m_header(header)
{
    _code = m_header.substr(0, 3);
    setResponse(_code);
}

void Response::setResponse(std::string message)
{
    m_response = "HTTP/1.1 " + message + "\n";
}

void Response::setServerName(std::vector<std::string> server_names)
{
    if (server_names[0] != "")
    {
        m_response += "server: " + server_names[0] + "\n";
    }
}

void Response::setContent(std::string content)
{
    m_response += content;
}

void Response::setErrorResponse(std::string message)
{
    m_response = message;
}

void Response::sendResponse()
{
    ssize_t bytes_sent = send(_socketFd, m_response.c_str(), m_response.size(), 0);
    if (bytes_sent <= 0)
    {
        printf("Error sending response\n");
    }
    printf("------------------Response sent-------------------\n");
    close(_socketFd);
}
