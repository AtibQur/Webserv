#ifndef SERVER_HPP
#define SERVER_HPP

#include "main.hpp"

class Client;
class Config;

class Server {
    private:
        int                 _server_fd;
        int                 _epoll;
        int                  _clientAcceptFd;
        struct sockaddr_in  _server_address,
                            _client_address;
        socklen_t           _addrlen;
        Config              *_conf;

        Client*             _client;
        std::string         _response;

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
        void createNewClient();
        void getRequest(struct epoll_event &event);
        void sendResponse(struct epoll_event &event);

        // GETTERS
        int getSockFd() const {return this->_server_fd; };
        int getAcceptFd() const { return this-> _clientAcceptFd; };
        Client* getServerClient() const { return this->_client; };

        // response 
        void createResponse(Client* client);
        void createErrorResponse(const std::string& errorMessage, Client *client);

        // get method functions
        void getMethod(Client* client);
        bool isPathAndMethodAllowed(Client* client);
        // post method functions
        void postMethod(Client *client);

};

#endif