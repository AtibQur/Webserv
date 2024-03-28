#include "main.hpp"

CgiToServer::CgiToServer(Client &client) : m_client(client)
{
    if (pipe(m_pipeFd) == -1)
        perror("pipeOut error");
    m_socketFd = m_pipeFd[READ];
}

CgiToServer::~CgiToServer()
{
    if (m_pipeFd[READ] != -1)
        close(m_pipeFd[READ]);
    if (m_pipeFd[WRITE] != -1)
        close(m_pipeFd[WRITE]);
}

void CgiToServer::readFromPipe()
{
    char buf[BUFSIZ] = {0};

    int bytes_read = read(m_pipeFd[READ], &buf, BUFSIZ - 1);
    if (bytes_read == -1) 
    {
        throw std::invalid_argument("500");
    }
    else
    {
        m_client._response.setResponse("200 OK");
        m_client._response.setContent("Content-Length: " + std::to_string(bytes_read) + "\n\n" + buf);
    }
    m_client.removeFromEpoll(m_pipeFd[READ]);
    m_client.modifyEpoll(&m_client, EPOLLOUT, m_client.m_socketFd);
}