#ifndef CLIENT_HPP
#define CLIENT_HPP

class Client
{
private:
    int clientSocket;

	// request line information
	std::string _method;
	std::string _uri;
	std::string _protocol;
	// header information
	std::map <std::string, std::string> headerMap;
	// request body
	std::string _boundary;
public:
    Client();
    Client(int client_socket);
    ~Client();
    Client(Client const &copy);
    Client &operator=(Client const &copy);

    void	saveClientRequest(char* buffer, int client_socket);
	void	parseRequest(char* buffer);

	// getters
	int			getClientSocket() { return clientSocket; };
	std::string getMethod() { return _method; };
  int			getNbMethod();
	std::string getUri() { return _uri; };
	std::string getProtocol() { return _protocol; };
};

#endif