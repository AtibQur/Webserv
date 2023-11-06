#ifndef SERVER_HPP
#define SERVER_HPP

#include "main.hpp"

class Client;
class Config;

class Server {
    private:
        int                 _server_fd;
        int                 _new_socket;
        struct sockaddr_in  _address;
        int _addrlen = sizeof(_address);
        Config              *_conf;

        int                 _epoll_fd;
        int                 _MAX_EVENTS;
        int                 _MAX_CLIENTS;
        std::vector<int>    _client_sockets; // To keep track of client sockets
        int                 _optval;
        
    public:
        Server();
        Server(Config *conf);
        ~Server();
        Server(Server const &copy);
        Server &operator=(Server const &copy);

        void Start();
        void CreateSocket();
        void BindSocketToPort();
        void ListenToSocket();
        void initEpoll();

        // response 
        bool isRequestComplete(std::string accumulatedRequestData);
        void createResponse(Client* client);

        // get method functions
        void getMethod(Client* client);
        int isMethodAllowed(Client* client);
        // post method functions
        void postMethod();

};

#endif