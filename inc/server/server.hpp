#ifndef SERVER_HPP
#define SERVER_HPP

class Client;

class Server
{

    private:
        int _server_fd;
        int _client_socket;

    public:
        Server();
        ~Server();
        Server(Server const &copy);
        Server &operator=(Server const &copy);

        void start();
        void createRespond(Client* client);
    };


#endif