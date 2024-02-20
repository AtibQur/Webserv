#include "main.hpp"

CgiToServer::CgiToServer(Client &client) : m_client(client) {
    if (pipe(m_pipeFd) == -1)
        perror ("pipeOut error");
    m_socketFd = m_pipeFd[READ];
}

CgiToServer::~CgiToServer() {
    if (m_pipeFd[READ] != -1)
        close(m_pipeFd[READ]);
    if (m_pipeFd[WRITE] != -1)
        close(m_pipeFd[WRITE]);
}

void CgiToServer::readFromPipe() {
    char buf[BUFSIZ] = {0};

    std::cout << "Cgi reading from pipe: " << m_pipeFd[READ] << std::endl;

    size_t bytes_read = read(m_pipeFd[READ], &buf, BUFSIZ - 1);
    if (bytes_read <= 0)
        perror ("Error reading from pipe");
    else {
        //?
        std::cerr << "Buf = " << buf << std::endl;
        // m_client.getResponse().setContent(buf);
        m_client._response.setResponse("200 OK");
        m_client._response.setContent("14\n\nFile not found");

        std::cerr << "response message: " << m_client.getResponse().getResponseMessage() << std::endl;
    }

    m_client.removeFromEpoll(m_pipeFd[READ]);
    m_client.modifyEpoll(&m_client, EPOLLOUT, m_client.m_socketFd);
}