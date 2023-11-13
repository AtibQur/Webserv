#ifndef SERVER_HPP
#define SERVER_HPP

#include "main.hpp"

class Client;
class Config;

class Server {
    private:
        int                 _server_fd;
        int                 _addrlen;
        int                 _epoll;
        int                 _acceptFd;
        struct sockaddr_in  _server_address,
                            _client_address;
        Config              *_conf;
        // int                 _MAX_EVENTS;
        // std::vector<int>    _client_sockets; // To keep track of client sockets

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

        void initServerEpoll(int epoll);
        void clientAccept(int eventFd);
        void getRequest(int eventFd);

        // GETTERS
        int getSockFd() const {return this->_server_fd; };
        int getAcceptFd() const { return this->_acceptFd; };

        // response 
        void createResponse(Client* client);
        void createErrorResponse(const std::string& errorMessage, Client *client);

        // get method functions
        void getMethod(Client* client);
        bool isPathAndMethodAllowed(Client* client);
        // post method functions
        void postMethod();

};

#endif