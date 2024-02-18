#ifndef CGIIN_HPP
#define CGIIN_HPP

#include "main.hpp"

#define READ 0
#define WRITE 1

class Client;

class CgiIn : public Socket {
public:
    int        m_pipeFd[2]{};
    Client     &m_client;

    CgiIn() = delete;
    CgiIn(Client &client);
    ~CgiIn();
    void    WriteCgi();
};

#endif