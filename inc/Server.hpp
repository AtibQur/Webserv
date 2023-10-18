#ifndef SERVER_HPP
#define SERVER_HPP

class Client;

class Server {
    private:
        int                 _server_fd;
        int                 _new_socket;
        struct sockaddr_in  _address;
        int _addrlen = sizeof(_address);

        int                 _epoll_fd;
        int                 _MAX_EVENTS;
        int                 _MAX_CLIENTS;
        std::vector<int>    _client_sockets; // To keep track of client sockets

        int                 _optval;
    public:
        Server();
        ~Server();
        Server(Server const &copy);
        Server &operator=(Server const &copy);

        void Start();
        void CreateSocket();
        void BindSocketToPort();
        void ListenToSocket();
        void initEpoll();

        void createRespond(Client* client);
};

#endif