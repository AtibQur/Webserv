#include "../../inc/main.hpp"

/* Default constructor/destructor */

Server::Server() {
    _MAX_EVENTS = 1000;
    _MAX_CLIENTS = 1000;
}


Server::Server(Config *conf) : _conf(conf) {
    this->initServer();
}

void Server::initServer() {
    this->initSocketFd();
    this->initAddress();
    this->initSocketOpt();
    this->initNonBlock();
    this->BindSocket();
    _addrlen =  sizeof(_server_address);
}

Server::~Server() {
    close(this->_new_socket);
    close(this->_server_fd);
    std::cout << "Server closed" << std::endl;
}

Server::Server(Server const &copy) {
    *this = copy;
}

Server& Server::operator=(Server const &copy) {
    *this = copy;
    return *this;
}

/* Server start */

void Server::Start() {

    std::cout << "Server started, listening on port " << PORT << "..." << std::endl;

        while (1) {
        //  wait for events on file descriptors registered with epoll
        struct epoll_event events[_MAX_EVENTS];
        int num_events = epoll_wait(_epoll_fd, events, _MAX_EVENTS, -1);
        if (num_events < 0) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < num_events; ++i) {
            if (events[i].data.fd == _server_fd) {
                // New client connection
                if ((this->_new_socket = accept(this->_server_fd, (struct sockaddr *)&_address, (socklen_t *)&_addrlen)) < 0) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                // Add the new client socket to epoll
                struct epoll_event client_event;
                client_event.events = EPOLLIN;
                client_event.data.fd = this->_new_socket;
                if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, this->_new_socket, &client_event) < 0) {
                    perror("epoll_ctl (client)");
                    exit(EXIT_FAILURE);
                }
                this->_client_sockets.push_back(this->_new_socket);
            } else {
                // Existing client socket ready for read
                int client_socket = events[i].data.fd;
                // read from the client request
                char buffer[1024] = {0};
                ssize_t bytes_read;
                std::string accumulatedRequestData;

                while (1) {
                    bytes_read = read(client_socket, buffer, sizeof(buffer));

                    std::string HardcodedRequest = buffer; // hardcoded the string to work with for the response

                    if (bytes_read < 0) {
                        perror("Error reading from the client socket");
                        break ;
                    } else if (bytes_read == 0){
                        std::cout << "Connection closed by the client." << std::endl;
                        break ;
                    }
                    else {
                        accumulatedRequestData.append(buffer, bytes_read); // append the request and break when it's complete

                        if (isRequestComplete(accumulatedRequestData)){
                            Client* client = new Client(client_socket); // create a client class

                            client->handleRequest(accumulatedRequestData, buffer); // parse the request with this client
                            createResponse(client); // send a response
                            delete client; // delete the client after the response
                            close (client_socket); // close the connection (still need to delete the socket from the _client_sockets);
                            break ;
                        }
                    }
                }
            }
        }
    }
}

/* Create a socket */

void Server::initSocketFd() {
    if ((this->_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
}

/* Set up signal handler*/

void Server::initSocketOpt() {
    int optval = 1;

    int opt = setsockopt(_server_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)); // handle signals (ctrl+C)
    if (opt < 0) {
        perror("setsockopt() failed");
        close(this->_server_fd);
        exit(EXIT_FAILURE);
    }
}

/* To make server socket non blocking*/

void Server::initNonBlock() {
    int result = fcntl(this->_server_fd, F_SETFL, O_NONBLOCK);
    if (result < 0) {
        perror("initNonBlock failed()");
        close(this->_server_fd);
        exit(EXIT_FAILURE);
    }
}

/* assign socket addres and give the socket the adress/netwerk and port numbers */

void Server::initAddress() {
    bzero((char*)&_server_address, sizeof(_server_address)); // clear memory of server address
    _server_address.sin_family = AF_INET;
    _server_address.sin_addr.s_addr = INADDR_ANY;
    _server_address.sin_port = htons(PORT);
    memset(_server_address.sin_zero, '\0', sizeof(_server_address)); // set remaining bytes to zero

}

/* Bind socket to server address*/

void Server::BindSocket() {
    if (bind(this->_server_fd, (struct sockaddr *)&_server_address, sizeof(this->_server_address)) < 0) {
        perror("bind failed");
        close(this->_server_fd);
        exit(EXIT_FAILURE);
    }
}

/* listen to incoming socket connection and have a max amount of people in queue */

void Server::ListenToSocket() {
    if (listen(this->_server_fd, _MAX_CLIENTS) < 0) {
        perror("listen() failed");
        close(this->_server_fd);
        exit(EXIT_FAILURE);
    }
}

/* Initialise epoll and add server to it*/

void Server::initEpoll() {
    // Create epoll instance
    this->_epoll_fd = epoll_create1(0);
    if (this->_epoll_fd < 0) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    // Add server socket to epoll
    struct epoll_event event;
    event.events = EPOLLIN; // Watch for input events
    event.data.fd = this->_server_fd;
    if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, this->_server_fd, &event) < 0) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }
}



                    // // Remove the client socket from epoll and close it
                    // epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, client_socket, nullptr);
                    // close(client_socket);
                    // // Remove the client socket from the list
                    // for (auto it = _client_sockets.begin(); it != _client_sockets.end(); ++it) {
                    //     if (*it == client_socket) {
                    //         _client_sockets.erase(it);
                    //         break;
                    //     }
                    // }