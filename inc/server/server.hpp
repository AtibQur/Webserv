#ifndef SERVER_HPP
#define SERVER_HPP

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

        // SIGNAL HANDLER
        // void SignalInterrupt();
    };


#endif