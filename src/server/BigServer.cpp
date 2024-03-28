#include "../../inc/main.hpp"

int g_state = 1;

static void handleSignal(int signal)
{
    (void)signal;
    g_state = 0;
}

BigServer::BigServer()
{
    _MAX_EVENTS = 10;
    std::cout << "Default bigServer constructor" << std::endl;
}

BigServer::BigServer(std::vector<Config> newConfig) : _config(newConfig)
{

    size_t size = _config.size();
    for (size_t i = 0; i < size; i++)
    {
        Server *server = new Server(&_config[i]);
        std::cout << "New server created" << std::endl;
        _server.push_back(server);
    }
    runBigServer();
}

BigServer::~BigServer()
{
    std::cout << "Big Server closed" << std::endl;
}

void BigServer::runBigServer()
{
    signal(SIGINT, handleSignal);
    initEpoll();
    while (g_state)
    {
        setupNewEvents();
        loopEvents();
    }

    close(_epoll);
    std::cout << "Server closed" << std::endl;
}

void BigServer::loopEvents()
{
    struct epoll_event event;
    Socket *epollPtr{};

    for (int i = 0; i < _num_events; i++)
    {
        event = _events[i];
        epollPtr = static_cast<Socket *>(event.data.ptr);
        if (epollPtr == nullptr)
            std::cout << "epollPtr Error" << std::endl;

        if (event.events & EPOLLIN)
        {
            incomingRequest(epollPtr);
        }
        else if (event.events & EPOLLOUT)
        {
            outgoingResponse(epollPtr);
        }
    }
}

void BigServer::incomingRequest(Socket *ptr)
{
    if (Client *client = dynamic_cast<Client *>(ptr))
    {
        client->receiveRequest();
    }
    if (Server *server = dynamic_cast<Server *>(ptr))
    {
        connectNewClient(server);
    }
    if (CgiToServer *cgiToServer = dynamic_cast<CgiToServer *>(ptr))
    {
        cgiToServer->readFromPipe();
    }
}

void BigServer::connectNewClient(Server *server)
{
    Client *client = new Client(*server, server->getConf()->getErrorPages(), server->getConf()->getLocations());
    _client.push_back(client);
    std::cout << "New client connected" << std::endl;
    if (!client)
    {
        perror("no client connected");
        return;
    }
    client->setEpoll(server->getEpoll()); // TODO in try and catch
    struct epoll_event event;
    event.events = EPOLLIN;

    Socket *ptr = client;
    event.data.ptr = ptr;

    if (epoll_ctl(_epoll, EPOLL_CTL_ADD, client->getSocketFd(), &event) == -1)
    {
        perror("epoll_ctl client");
    }
}

void BigServer::modEpoll(int fd)
{
    struct epoll_event event;
    event.events = 0;
    event.data.ptr = nullptr;
    if (epoll_ctl(_epoll, EPOLL_CTL_MOD, fd, &event) == -1)
    {
        std::cerr << "Error modifying epoll" << std::endl;
        // setError(m_socketFd, "500 Internal Server Error");
    }
}

void BigServer::outgoingResponse(Socket *ptr)
{
    if (Client *client = dynamic_cast<Client *>(ptr))
    {
        client->handleResponse();
    }
    else if (ServerToCgi *serverToCgi = dynamic_cast<ServerToCgi *>(ptr))
    {
        modEpoll(serverToCgi->m_pipeFd[WRITE]);
        if (epoll_ctl(_epoll, EPOLL_CTL_DEL, serverToCgi->m_pipeFd[WRITE], NULL) == -1)
            perror("remove epoll");

        serverToCgi->WriteCgi();
        serverToCgi->m_client.addCGIProcessToEpoll(&(serverToCgi->m_client.getcgiToServer()), EPOLLIN, serverToCgi->m_client.getcgiToServer().m_pipeFd[READ]);
        serverToCgi->m_client.handleCGI();
    }
}

int BigServer::findServerIndex(int eventFd)
{
    int index = 0;

    for (auto &server : _server)
    {
        if (server->getSockFd() == eventFd)
        {
            return index;
        }
        index++;
    }
    return 0;
}

void BigServer::setupNewEvents()
{
    _num_events = epoll_wait(getEpoll(), _events, 10, -1);
    if (_num_events == -1)
    {
        std::cout << "Error in epoll_wait" << std::endl;
        perror("epoll_wait");

        for (auto &server : _server)
        {
            close(server->getSockFd());
            delete server;
        }
        for (auto &client : _client)
        {
            delete client;
        }
        close(103);
        close(40);
        close(38);
    }
}

void BigServer::initEpoll()
{
    _epoll = epoll_create1(0);
    if (_epoll == -1)
    {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    for (auto &server : _server)
    {
        struct epoll_event event;
        event.events = EPOLLIN;

        Socket *ptr = server;
        event.data.ptr = ptr;

        if (epoll_ctl(_epoll, EPOLL_CTL_ADD, server->getSockFd(), &event) == -1)
        {
            perror("epoll_ctl server");
            exit(EXIT_FAILURE);
        }
        server->initServerEpoll(_epoll);
    }
}
