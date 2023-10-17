#ifndef _SERVER_HPP_
#define _SERVER_HPP_

class Server {
    private:
        int server_fd;
        int epoll_fd;
        std::vector<int> client_sockets; // To keep track of client sockets
        int new_socket;
        struct sockaddr_in address;
        int addrlen = sizeof(address);

    public:
        Server();
        ~Server();
        void Start();

        // PORT/SOCKET SETTER
        void CreateSocket();
        void BindSocketToPort();
        void ListenToSocket();

        // EPOLL
        void initEpoll();
};

#endif