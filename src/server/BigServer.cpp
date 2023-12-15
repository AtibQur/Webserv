#include "../../inc/main.hpp"

BigServer::BigServer() {
    _MAX_EVENTS = 10;
    std::cout << "Default bigServer constructor" << std::endl;
}

BigServer::BigServer(std::vector<Config> newConfig) : _config(newConfig) {

    size_t size = _config.size();
    for (size_t i = 0; i < size; i++) {
        Server *server = new Server(&_config[i]);
        _server.push_back(server);
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
    Socket *epollPtr{};

    for (int i = 0; i < _num_events; i++) {

        event = _events[i];
        epollPtr = static_cast<Socket *>(event.data.ptr);
        if (epollPtr == nullptr)
            std::cout << "epollPtr error" << std::endl;

        if (event.events & EPOLLIN) 
        {   
           ConnectClient(epollPtr);
        } 
        else if (event.events & EPOLLOUT) 
        {    
            if (Client *client = dynamic_cast<Client *>(epollPtr)){
                std::cout << "client creating response" << std::endl;
                client->sendResponse();
            }
        }
    }
}

void BigServer::ConnectClient(Socket *ptr) {
    if (Client *client = dynamic_cast<Client *>(ptr)){
        // std::cout << "existing client ready for read" << "\n";
        client->readBuffer();
    }
    if (Server *server = dynamic_cast<Server *>(ptr)){
        // std::cout << "new client" << std::endl;
        ConnectNewClient(server, _eventFd);
    }
}

void BigServer::ConnectNewClient(Server *server, int eventFd) 
{
    Client *client = new Client(*server, server->getConf()->getErrorPages(), server->getConf()->getLocations());
    
    client->setEpoll(server->getEpoll());
    struct epoll_event event;
    event.events = EPOLLIN;

    Socket *ptr = client;
    event.data.ptr = ptr;

    if (epoll_ctl(_epoll, EPOLL_CTL_ADD, client->getSocketFd(), &event) == -1) {
        perror("epoll_ctl client"); 
        exit(EXIT_FAILURE);
    }

    // std::cout << "new client added to epoll" << std::endl;
}

// void BigServer::ConnectNewClient(int index, int eventFd) 
// {
//     _server[index].clientAccept(eventFd);
//     _server[index].createNewClient();
// }

// void BigServer::loopEvents() {
//     struct epoll_event event;
//     for (int i = 0; i < _num_events; i++) {
//         event = _events[i];
//         _eventFd = event.data.fd;
//         int index = findServerIndex(_eventFd);
//         if (index < 0) {
//             perror("Index not found");
//             exit(EXIT_FAILURE);
//         }
//         _server[index].clientAccept(_eventFd);
//         _server[index].getRequest(_eventFd);
//     }
// }

int BigServer::findServerIndex(int eventFd) {
    int index = 0;

    for (auto& server : _server) {
        if (server->getSockFd() == eventFd) {
            return index;
        }
        index++;
    }
    return 0;
}

void BigServer::setupNewEvents() {
    _num_events = epoll_wait(getEpoll(), _events, 10, -1);
    if (_num_events == -1) {
        perror("epoll_wait");
        exit(EXIT_FAILURE);
    }
}

void BigServer::initEpoll() {
    _epoll = epoll_create1(0);
    if (_epoll == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    for (auto& server : _server) {
        struct epoll_event event;
        event.events = EPOLLIN;

        Socket *ptr = server;
        event.data.ptr = ptr;

        if (epoll_ctl(_epoll, EPOLL_CTL_ADD, server->getSockFd(), &event) == -1) {
            perror("epoll_ctl server"); 
            exit(EXIT_FAILURE);
        }
        server->initServerEpoll(_epoll);
    }
}
