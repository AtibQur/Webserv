#include "main.hpp"

ServerToCgi::ServerToCgi(Client &client) : m_client(client) {
    if (pipe(m_pipeFd) == -1)
        perror ("pipeOut error");
    m_socketFd = m_pipeFd[WRITE];
}

ServerToCgi::~ServerToCgi() {
    if (m_pipeFd[READ] != -1)
        close(m_pipeFd[READ]);
    if (m_pipeFd[WRITE] != -1)
        close(m_pipeFd[WRITE]);
}

void ServerToCgi::WriteCgi() {

    if (dup2(m_pipeFd[READ], STDIN_FILENO) == -1)
        perror("dub2 error");
    if (close(m_pipeFd[READ]) == -1)
        perror("close read error");
    m_pipeFd[READ] = -1;


    const char *temp = "3\n\nHOI";
    write(STDIN_FILENO, temp, strlen(temp)); // write to stdin
    //TODO check for bytes writen is not -1

    if (close(m_pipeFd[WRITE]) == -1)
        perror("close write error");

    // if (bytes <= 0)
    //     perror (" write bytes error");
    // else
        std::cout << "write cgi response " << std::endl;
    m_pipeFd[WRITE] = -1;
}