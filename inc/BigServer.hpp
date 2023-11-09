#ifndef BIGSERVER_HPP
#define BIGSERVER_HPP

#include "main.hpp"

class BigServer
{
private:
    std::vector<Config> _config;
    std::vector<Server> _server;

    int _epoll;

public:
    BigServer();
    BigServer(std::vector<Config> config);
    ~BigServer();

    void runBigServer();
    void initEpoll();
};

#endif