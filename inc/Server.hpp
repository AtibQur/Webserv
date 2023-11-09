#ifndef SERVER_HPP
#define SERVER_HPP

#include "main.hpp"

class Client;
class Config;

class Server {
    private:
        int                 _server_fd;
        int                 _new_socket;
        int                 _addrlen;
        struct sockaddr_in  _server_address;
        Config              *_conf;

        int                 _epoll_fd;
        int                 _MAX_EVENTS;
        int                 _MAX_CLIENTS;
        std::vector<int>    _client_sockets; // To keep track of client sockets

        
    public:
        Server();
        Server(std::vector<Config> conf);
        Server(Config *conf);
        ~Server();
        Server(Server const &copy);
        Server &operator=(Server const &copy);

        void initServer();
        // void Start();
        void initSocketFd();
        void initAddress();
        void initSocketOpt();
        void initNonBlock();
        void BindSocket();
        void ListenToSocket();
        // void initEpoll();

        // response 
        bool isRequestComplete(std::string accumulatedRequestData);
        void createResponse(Client* client);
        void createErrorResponse(const std::string& errorMessage, Client *client);

        // get method functions
        void getMethod(Client* client);
        bool isPathAndMethodAllowed(Client* client);
        // post method functions
        void postMethod();

};

#endif