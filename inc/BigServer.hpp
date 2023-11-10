#ifndef BIGSERVER_HPP
#define BIGSERVER_HPP

#include "main.hpp"

class Config;
class Server;

class BigServer
{
private:
    std::vector<Config> _config;
    std::vector<Server> _server;

    int _epoll;
    int _num_events;
    int _MAX_EVENTS;
    int _eventFd;

    struct epoll_event _events[10];

public:
    BigServer();
    BigServer(std::vector<Config> config);
    ~BigServer();

    void runBigServer();
    void initEpoll();
    void setupNewEvents();
    void loopEvents();

    int findServerIndex(int eventFd);
    int getEpoll() { return this->_epoll; };
};

#endif