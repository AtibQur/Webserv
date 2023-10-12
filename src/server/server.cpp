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
    long valread;
    int addrlen = sizeof(address);

    char hello[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

    // create a socket
    if ((_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

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
        exit(EXIT_FAILURE);
    }
    if (listen(_server_fd, 10) < 0) // listen to incoming socket connection and have a max amount of people in queu
    {
        perror("In listen");
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
        
        char buffer[30000] = {0};
        valread = read( _client_socket , buffer, 30000);
        printf("%s\n",buffer );
        write(_client_socket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------\n");
        close(_client_socket);
    }
}
