#ifndef CLIENT_HPP
#define CLIENT_HPP

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

	void readBuffer();

	// parser
    void	saveClientRequest(char* buffer, int client_socket);
	int		handleRequest(std::string request, char *buffer);
	bool	checkRequestLine(std::string httpRequest);
	bool	checkMethod(std::string tmp);
	void	createErrorResponse(const std::string& errorMessage);
	int		parseRequest(std::string request, char* buffer);

	bool isRequestComplete(std::string accumulatedRequestData);
	int getSocketFd() const { return this->_socketFd; };

	// getters
	// int			getClientSocket() { return clientSocket; };
	std::string getMethod() { return _method; };
	int			getNbMethod();
	std::string getUri() { return _uri; };
	std::string getProtocol() { return _protocol; };
};

#endif