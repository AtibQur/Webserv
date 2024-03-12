#include "../../inc/main.hpp"

BigServer::BigServer() {
    _MAX_EVENTS = 10;
    std::cout << "Default bigServer constructor" << std::endl;
}

BigServer::BigServer(std::vector<Config> newConfig) : _config(newConfig) {

    size_t size = _config.size();
    for (size_t i = 0; i < size; i++) {
        Server *server = new Server(&_config[i]);
        std::cout << "New server created" << std::endl;
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

void BigServer::loopEvents() { //TODO in try and catch
    struct epoll_event event;
    Socket *epollPtr{};
    bool iscgi = true;

    for (int i = 0; i < _num_events; i++) {
        event = _events[i];
        epollPtr = static_cast<Socket *>(event.data.ptr);
        if (epollPtr == nullptr)
            std::cout << "epollPtr Error" << std::endl;

        if (event.events & EPOLLIN)
        {
            incomingRequest(epollPtr); // read
        }
        else if (event.events & EPOLLOUT)
        {   
            outgoingResponse(epollPtr); // write
        }
    }
}

void BigServer::incomingRequest(Socket *ptr) {
    if (Client *client = dynamic_cast<Client *>(ptr)){
        client->receiveRequest();
    }
    if (Server *server = dynamic_cast<Server *>(ptr)){
        connectNewClient(server, _eventFd);
    }
    if (CgiToServer *cgiToServer = dynamic_cast<CgiToServer *>(ptr)){ //! EPOLLIN for CGI
        std::cout << "CGI EPOLLIN \n";
        cgiToServer->readFromPipe();
    }
}

void BigServer::connectNewClient(Server *server, int eventFd) 
{
    Client *client = new Client(*server, server->getConf()->getErrorPages(), server->getConf()->getLocations());
    std::cout << "New client connected" << std::endl;
    if (!client) {
        perror("no client connected");
        return ;
    }
    client->setEpoll(server->getEpoll()); // add client to EPOLLIN //TODO in try and catch
    struct epoll_event event;
    event.events = EPOLLIN;

    Socket *ptr = client;
    event.data.ptr = ptr;

    if (epoll_ctl(_epoll, EPOLL_CTL_ADD, client->getSocketFd(), &event) == -1) {
        perror("epoll_ctl client"); 
    }
}

void BigServer::modEpoll(int fd) {
    struct epoll_event event;
    event.events = 0;
    event.data.ptr = nullptr;
    if (epoll_ctl(_epoll, EPOLL_CTL_MOD, fd, &event) == -1)
    {
        std::cerr << "Error modifying epoll" << std::endl;
        // setError(m_socketFd, "500 Internal Server Error");
    }
}

void BigServer::outgoingResponse(Socket *ptr) {
    if (Client *client = dynamic_cast<Client *>(ptr)) 
    {
        client->handleResponse();
    }
    else if (ServerToCgi *serverToCgi = dynamic_cast<ServerToCgi *>(ptr)) //! EPOLOUT for CGI
    {
        std::cout << "CGI EPOLLOUT \n";
        modEpoll(serverToCgi->m_pipeFd[WRITE]); //TODO new function
        if (epoll_ctl(_epoll, EPOLL_CTL_DEL, serverToCgi->m_pipeFd[WRITE], NULL) == -1)
            perror ("remove epoll");

        serverToCgi->WriteCgi();
        serverToCgi->m_client.addCGIProcessToEpoll(&(serverToCgi->m_client.getcgiToServer()), EPOLLIN, serverToCgi->m_client.getcgiToServer().m_pipeFd[READ]);
        serverToCgi->m_client.handleCGI();
    }
}

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
