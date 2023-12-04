#ifndef SERVER_HPP
#define SERVER_HPP

#include "main.hpp"

class Client;
class Config;

class Server : public Socket {
    private:
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
        void AddClientToEpoll();
        void createNewClient();
        void getRequest(struct epoll_event &event);
        void sendResponse(struct epoll_event &event);

        // GETTERS
        Config *getConf() const { return this->_conf; };
        int getSockFd() const {return this->m_socketFd; };
        int getAcceptFd() const { return this-> _clientAcceptFd; };
        Client* getServerClient() const { return this->_client; };
        int getEpoll() const { return this->_epoll; };
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