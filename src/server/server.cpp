#include "../../inc/main.hpp"

/* Default constructor/destructor */

Server::Server() {}

Server::~Server() {
    close(this->_client_socket);
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

void Server::start() {
    
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // create a socket
    if ((_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
void Server::Start() {
    // Message for client
    char response[] = "HTTP/1.1 200\nContent-Type: text/plain\nContent-Length: 37\n\nHello mi brothas\nHope all is well :)\n";

    this->CreateSocket();
    this->BindSocketToPort();
    this->ListenToSocket();
    this->initEpoll();

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
                // Handle data on the client socket and send a response
                char buffer[1024];
                ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer));
                if (bytes_read < 0) {
                    perror("read");
                } else if (bytes_read == 0) {
                    // Connection closed
                    std::cout << "Connection closed by the client." << std::endl;
                    // Remove the client socket from epoll and close it
                    epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, client_socket, nullptr);
                    close(client_socket);
                    // Remove the client socket from the list
                    for (auto it = _client_sockets.begin(); it != _client_sockets.end(); ++it) {
                        if (*it == client_socket) {
                            _client_sockets.erase(it);
                            break;
                        }
                    }
                } else {
                    // Process the received data and send a response
                    std::cout << "Received data: " << std::string(buffer, bytes_read) << std::endl;
                    send(client_socket, response, strlen(response), 0);
                }
            }
        }
    }
}

/* Create a socket and also set up signal handler */

void Server::CreateSocket() {
    if ((this->_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // allowing the server to re-use the same port
    int optval = 1;
    setsockopt(_server_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    // assign socket adress
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    memset(address.sin_zero, '\0', sizeof address.sin_zero);


    if (bind(_server_fd, (struct sockaddr *)&address, sizeof(address))<0) // give the socket the adress/netwerk and port numbers 
    {
        perror("In bind");
    setsockopt(_server_fd, SOL_SOCKET, SO_REUSEPORT, &_optval, sizeof(_optval)); // handle signals (ctrl+C)
}

/* assign socket addres and give the socket the adress/netwerk and port numbers */

void Server::BindSocketToPort() {
    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = INADDR_ANY;
    this->_address.sin_port = htons(PORT);
    if (bind(this->_server_fd, (struct sockaddr *)&_address, sizeof(this->_address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(_server_fd, 10) < 0) // listen to incoming socket connection and have a max amount of people in queu
    {
        perror("In listen");
}

/* listen to incoming socket connection and have a max amount of people in queue */

void Server::ListenToSocket() {
    if (listen(this->_server_fd, _MAX_CLIENTS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        printf("\n+++++++ Waiting for client connection ++++++++\n\n");
        if ((_client_socket = accept(_server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        // read and parse the client request and save client pointer in a list
        Client client;
        client.saveClientRequest(_client_socket);

        // create a respond (WIP)
        createRespond(client.getClientListIndex(0));
        close(_client_socket);
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

void Server::createRespond(Client* client) {
    std::cout << client->getMethod() << std::endl;
    char hello[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    write(_client_socket , hello , strlen(hello));
    printf("------------------Hello message sent-------------------\n");
}
