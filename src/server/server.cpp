#include "../../inc/main.hpp"

Server::Server() {
}

Server::~Server() {
    close(this->new_socket);
    close(this->server_fd);
}

// PORT/SOCKET SETTER

void Server::CreateSocket() {
    if ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
}

void Server::BindSocketToPort() {
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(PORT);
    if (bind(this->server_fd, (struct sockaddr *)&address, sizeof(this->address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void Server::ListenToSocket() {
    if (listen(this->server_fd, 1000) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

void Server::Start() {
    // Bericht voor de client
    char response[] = "HTTP/1.1 200\nContent-Type: text/plain\nContent-Length: 37\n\nHello mi brothas\nHope all is well :)\n";

    // Maak een socket
    this->CreateSocket();

    int optval = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    // Bind socket aan poort 8080
    this->BindSocketToPort();

    // Luister naar de socket
    this->ListenToSocket();

    std::cout << "Server started, listening on port " << PORT << "..." << std::endl;

    // Initialize epoll
    this->initEpoll();

        while (1) {
        const int MAX_EVENTS = 1000;
        struct epoll_event events[MAX_EVENTS];
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_events == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < num_events; ++i) {
            if (events[i].data.fd == server_fd) {
                // New client connection
                if ((this->new_socket = accept(this->server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                // Add the new client socket to epoll
                struct epoll_event client_event;
                client_event.events = EPOLLIN;
                client_event.data.fd = this->new_socket;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, this->new_socket, &client_event) == -1) {
                    perror("epoll_ctl (client)");
                    exit(EXIT_FAILURE);
                }
                this->client_sockets.push_back(this->new_socket);
            } else {
                // Existing client socket ready for read
                int client_socket = events[i].data.fd;
                // Handle data on the client socket and send a response
                char buffer[1024];
                ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer));
                if (bytes_read == -1) {
                    perror("read");
                } else if (bytes_read == 0) {
                    // Connection closed
                    std::cout << "Connection closed by the client." << std::endl;
                    // Remove the client socket from epoll and close it
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_socket, nullptr);
                    close(client_socket);
                    // Remove the client socket from the list
                    for (auto it = client_sockets.begin(); it != client_sockets.end(); ++it) {
                        if (*it == client_socket) {
                            client_sockets.erase(it);
                            break;
                        }
                    }
                } else {
                    // Process the received data
                    std::cout << "Received data: " << std::string(buffer, bytes_read) << std::endl;
                    // Send a response
                    send(client_socket, response, strlen(response), 0);
                }
            }
        }
    }
}

// EPOLL
void Server::initEpoll() {
    // Create epoll instance
    this->epoll_fd = epoll_create1(0);
    if (this->epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    // Add server socket to epoll
    struct epoll_event event;
    event.events = EPOLLIN; // Watch for input events
    event.data.fd = this->server_fd;

    if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, this->server_fd, &event) == -1) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }
}