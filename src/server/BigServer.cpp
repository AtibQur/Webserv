#include "../../inc/main.hpp"

BigServer::BigServer() {

}

BigServer::BigServer(std::vector<Config> newConfig) : _config(newConfig) {

    size_t size = _config.size();

    for (size_t i = 0; i < size; i++) {
        Server tmp(&_config[i]);
        _server.push_back(tmp);
    }
    runBigServer();
}

BigServer::~BigServer() {}


void BigServer::runBigServer() {
    initEpoll();
}

void BigServer::initEpoll() {
    _epoll = epoll_create1(0);
    if (_epoll < 0) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    struct epoll_event event;
    event.events = EPOLLIN;
    // event.data.fd = this->_server_fd;
    // if (epoll_ctl(this->_epoll, EPOLL_CTL_ADD, this->_server_fd, &event) < 0) {
    //     perror("epoll_ctl");
    //     exit(EXIT_FAILURE);
    // }
}