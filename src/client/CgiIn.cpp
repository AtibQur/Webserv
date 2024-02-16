#include "main.hpp"

CgiIn::CgiIn() {
    if (pipe(m_pipeFd) == -1)
        perror ("pipeOut error");
    m_socketFd = m_pipeFd[WRITE];
}

CgiIn::~CgiIn() {
    if (m_pipeFd[READ] != -1)
        close(m_pipeFd[READ]);
    if (m_pipeFd[WRITE] != -1)
        close(m_pipeFd[WRITE]);
}
