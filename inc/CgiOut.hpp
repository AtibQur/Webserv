#ifndef CGIOUT_HPP
#define CGIOUT_HPP

#include "main.hpp"

#define READ 0
#define WRITE 1

class Client;

class CgiOut : public Socket {
public:
    int        m_pipeFd[2]{};
    Client     &m_client;

    CgiOut() = delete;
    CgiOut(Client &client);
    ~CgiOut();

    void readFromPipe();
    void execute();
};

#endif