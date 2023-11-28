#ifndef CLIENT_HPP
#define CLIENT_HPP

class Server;

class Client {
private:
    // int clientSocket;

	// request line information
	std::string _method;
	std::string _uri;
	std::string _protocol;
	// header information
	std::map <std::string, std::string> headerMap;
	// request body
	std::string _boundary;
	int 		_socketFd;

	std::string _requestBuffer;
public:
    Client();
    Client(int newSocketFd);
    ~Client();
    Client(Client const &copy);
    Client &operator=(Client const &copy);

	void readBuffer(Server* server);

	// parser
    void	saveClientRequest(char* buffer, int client_socket);
	void	handleRequest(Server* server, std::string request);
	bool	checkRequestLine(std::string httpRequest);
	bool	checkMethod(std::string tmp);
	int		parseRequest(std::string request);

	bool isRequestComplete(std::string accumulatedRequestData);
	int getSocketFd() const { return this->_socketFd; };

	// getters
	std::string getRequestBuffer() const { return this->_requestBuffer; };

	// int			getClientSocket() { return clientSocket; };
	std::string getMethod() { return _method; };
	int			getNbMethod();
	std::string getUri() { return _uri; };
	std::string getProtocol() { return _protocol; };
};

#endif