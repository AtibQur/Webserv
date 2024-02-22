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
    m_pipeFd[READ] = -1;

    std::cout << "What's in here?: " << m_client._response.getResponseMessage() << std::endl;
    write(m_pipeFd[WRITE], "3\n\nHoi", 7); // write to stdin

    // TODO check for bytes writen is not -1

    if (close(m_pipeFd[WRITE]) == -1)
        perror("close write error");
    std::cout << "write cgi response " << std::endl;
    // m_pipeFd[WRITE] = -1; //? cleanup
}

//! not working correctly