#include "../../inc/main.hpp"

BigServer::BigServer() {
    _MAX_EVENTS = 10;
    std::cout << "Default bigServer constructor" << std::endl;
}

BigServer::BigServer(std::vector<Config> newConfig) : _config(newConfig) {

    size_t size = _config.size();
    for (size_t i = 0; i < size; i++) {
        Server tmp(&_config[i]);
        _server.push_back(tmp);
    }
    runBigServer();
}

BigServer::~BigServer() {
    std::cout << "Big Server closed" << std::endl;
}


void BigServer::runBigServer() {
    initEpoll();
    while (1)
    {
        setupNewEvents();
        loopEvents();
    }
}

void BigServer::loopEvents() {
    struct epoll_event event;
    for (int i = 0; i < _num_events; i++) {
        event = _events[i];
        _eventFd = event.data.fd;
        int index = findServerIndex(_eventFd);
        if (index < 0) {
            perror("Index not found");
            exit(EXIT_FAILURE);
        } 
        // read:
        // server->client connection
        // client request
        // write:
        // client response
    }
}

int BigServer::findServerIndex(int eventFd) {
    int index = 0;

    for (auto& server : _server) {
        if (server.getSockFd() == eventFd) {
            return index;
        }
        index++;
    }

    return 0;
}

void BigServer::setupNewEvents() {
    _num_events = epoll_wait(getEpoll(), _events, 10, -1);
    if (_num_events < 0) {
        perror("epoll_wait");
        exit(EXIT_FAILURE);
    }
}

void BigServer::initEpoll() {
    _epoll = epoll_create1(0);
    if (_epoll < 0) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    for (auto& server : _server) {
        struct epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = server.getSockFd();
        if (epoll_ctl(_epoll, EPOLL_CTL_ADD, server.getSockFd(), &event) < 0) {
            perror("epoll_ctl");
            exit(EXIT_FAILURE);
        }
        server.initServerEpoll(_epoll);
    }
}
