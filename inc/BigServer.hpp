#ifndef BIGSERVER_HPP
#define BIGSERVER_HPP

#include "main.hpp"

class Config;
class Client;
class Server;

class BigServer
{
private:
    std::vector<Config>     _config;
    std::vector<Server *>   _server;
    std::vector<Client *>   _client;

    int _epoll;
    int _num_events;
    int _MAX_EVENTS;
    int _eventFd;

    struct epoll_event _events[10];

public:
    BigServer();
    BigServer(std::vector<Config> config);
    ~BigServer();

    void    runBigServer();
    void    initEpoll();
    void    setupNewEvents();
    void    loopEvents();
    void    incomingRequest(Socket *ptr);
    void    outgoingResponse(Socket *ptr);
    void    connectNewClient(Server *server);

    int     findServerIndex(int eventFd);
    int     getEpoll() { return this->_epoll; };

    void    modEpoll(int fd);
};

#endif