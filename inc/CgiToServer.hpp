#ifndef CGITOSERVER_HPP
#define CGITOSERVER_HPP

#include "main.hpp"

#define READ 0
#define WRITE 1

class Client;

class CgiToServer : public Socket
{
public:
    int m_pipeFd[2]{};
    Client &m_client;

    CgiToServer() = delete;
    CgiToServer(Client &client);
    ~CgiToServer();

    void readFromPipe();
    void execute();
};

#endif