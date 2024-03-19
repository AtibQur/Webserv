#include "main.hpp"

ServerToCgi::ServerToCgi(Client &client) : m_client(client)
{
    if (pipe(m_pipeFd) == -1)
        perror("pipeOut error");
    m_socketFd = m_pipeFd[WRITE];
}

ServerToCgi::~ServerToCgi()
{
    if (m_pipeFd[READ] != -1)
        close(m_pipeFd[READ]);
    if (m_pipeFd[WRITE] != -1)
        close(m_pipeFd[WRITE]);
}

void ServerToCgi::WriteCgi()
{
    if (dup2(m_pipeFd[READ], STDIN_FILENO) == -1)
        perror("dub2 error");
    if (close(m_pipeFd[READ]) == -1)
        perror("close read error");
    m_pipeFd[READ] = -1; //? cleanup

    std::string str = m_client.getCgiBody();
    write(m_pipeFd[WRITE], str.c_str(), strlen(str.c_str())); // write to stdin

    // TODO check for bytes writen is not -1

    if (close(m_pipeFd[WRITE]) == -1)
        perror("close write error");
    m_pipeFd[WRITE] = -1; //? cleanup
}

//! not working correctly
