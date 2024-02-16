#include "main.hpp"

// CgiOut::CgiOut() {
//     if (pipe(m_pipeFd) == -1)
//         perror ("pipeOut error");
//     m_socketFd = m_pipeFd[READ];
// }

CgiOut::CgiOut(Client &client) : m_client(client) {
    if (pipe(m_pipeFd) == -1)
        perror ("pipeOut error");
    m_socketFd = m_pipeFd[READ];
}

CgiOut::~CgiOut() {
    if (m_pipeFd[READ] != -1)
        close(m_pipeFd[READ]);
    if (m_pipeFd[WRITE] != -1)
        close(m_pipeFd[WRITE]);
}

void CgiOut::readFromPipe() {
    char buf[BUFSIZ]{};
    std::cout << "Cgi reading from pipe" << std::endl;
    int bytes_read{static_cast<int>(read(m_pipeFd[READ], &buf, BUFSIZ - 1))};
    if (bytes_read <= 0)
        perror ("reading from pipe");
    else {
        std::cout << buf << std::endl;
        m_client.getResponse().setResponse(buf);
    }

    m_client.removeFromEpoll(m_pipeFd[READ]);
    // m_client.modifyEpoll(&m_client, EPOLLOUT, m_client.m_socketFd);
}
