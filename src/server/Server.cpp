#include "../../inc/main.hpp"

/* Default constructor/destructor */

Server::Server() {
    std::cout << "Default constructor" << std::endl;
}


Server::Server(Config *conf) : _server_fd(-1), _conf(conf) {
    this->initServer();
}

void Server::initServer() {
    this->initAddress();
    this->initSocketFd();
    this->initSocketOpt();
    this->initNonBlock();
    this->BindSocket();
    this->ListenToSocket();
    _addrlen =  sizeof(_client_address);
}

Server::~Server() {
    // std::cout << "Server closed" << std::endl;
}

Server::Server(Server const &copy) {
    *this = copy;
}

Server& Server::operator=(Server const &copy) {
    this->_server_fd = copy._server_fd;
    this->_addrlen = copy._addrlen;
    this->_server_address = copy._server_address;
    this->_conf = copy._conf;
    this->_epoll = copy._epoll;
    this->_client_address = copy._client_address;

    return *this;
}

void Server::clientAccept(int eventFd) {

    _acceptFd = accept(eventFd, (struct sockaddr *)&_client_address, &_addrlen);
    if (_acceptFd == -1)
    {
        perror("client Accept() error");
        exit(EXIT_FAILURE);
    }
}

void Server::createNewClient(){
    _client = new Client(getAcceptFd());
    std::cout << "client created: " << _client->getSocketFd() << std::endl;
}

void Server::getRequest(int eventFd) {

    // Client *client = new Client(getAcceptFd());
    _client->readBuffer(this); // and respond
}

void Server::sendResponse(int eventFd){

    _client->handleRequest(this, _client->getRequestBuffer());
    std::cout << "send an response" << std::endl;
}

/* Create a socket */

void Server::initSocketFd() {
    _server_fd =  socket(AF_INET, SOCK_STREAM, 0);
    if (_server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
}

/* Set up signal handler*/

void Server::initSocketOpt() {
    int optval = 1;

    // reuseaddr
    int opt = setsockopt(getSockFd(), SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)); // handle signals (ctrl+C)
    if (opt == -1) {
        perror("setsockopt() failed");
        close(getSockFd());
        exit(EXIT_FAILURE);
    }
}

/* To make server socket non blocking*/

void Server::initNonBlock() {
    int result = fcntl(getSockFd(), F_SETFL, O_NONBLOCK);
    if (result < 0) {
        perror("initNonBlock failed()");
        close(getSockFd());
        exit(EXIT_FAILURE);
    }
}

/* assign socket addres and give the socket the adress/netwerk and port numbers */

void Server::initAddress() {
    bzero((char*)&_server_address, sizeof(_server_address)); // clear memory of server address
    _server_address.sin_family = AF_INET;
    _server_address.sin_addr.s_addr = INADDR_ANY;
    _server_address.sin_port = htons(_conf->getPort()); // Server PORT
    memset(_server_address.sin_zero, '\0', sizeof(_server_address)); // set remaining bytes to zero
}

/* Bind socket to server address*/

void Server::BindSocket() {
    if (bind(getSockFd(), (struct sockaddr *)&_server_address, sizeof(this->_server_address)) == -1) {
        perror("bind failed");
        close(getSockFd());
        exit(EXIT_FAILURE);
    }
}

/* listen to incoming socket connection and have a max amount of people in queue */

void Server::ListenToSocket() {
    if (listen(getSockFd(), 20) == -1) {
        perror("listen() failed");
        close(getSockFd());
        exit(EXIT_FAILURE);
    }
}

/* Set up access to big epoll */

void Server::initServerEpoll(int epoll) {
    _epoll = epoll;
}
