#ifndef SERVERTOCGI_HPP
#define SERVERTOCGI_HPP

#include "main.hpp"

#define READ 0
#define WRITE 1

class Client;

class ServerToCgi : public Socket {
public:
    int        m_pipeFd[2]{};
    Client     &m_client;

    ServerToCgi() = delete;
    ServerToCgi(Client &client);
    ~ServerToCgi();
    void    WriteCgi();
};

#endif