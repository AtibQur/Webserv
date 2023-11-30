#ifndef CLIENT_HPP
#define CLIENT_HPP

class Location;
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
	std::string _body;
	std::string _boundary;
	long long	_contentLength;
	std::string _fileNameBody;
	std::string _contentType;
	std::map <std::string, std::string> _error_pages;
	std::map <std::string, Location> _location;

	int 		_socketFd;

	std::string _requestBuffer;
public:
    Client();
    Client(int newSocketFd, std::map<std::string, std::string> ErrorPages, std::map<std::string, Location> Locations);
    ~Client();
    Client(Client const &copy);
    Client &operator=(Client const &copy);

	void	readBuffer(Server *server);
	void	createErrorResponse(const std::string& errorMessage);

	// parser
    void	saveClientRequest(char* buffer, int client_socket);
	bool	checkMethod(std::string tmp);
	void	handleRequest(Server *server, std::string request, char *buffer, ssize_t post);
	bool	checkRequestLine(std::string httpRequest);
	int		parseRequest(std::string request, char* buffer, ssize_t post);

	bool isRequestComplete(std::string accumulatedRequestData, ssize_t post);
	int getSocketFd() const { return this->_socketFd; };

	// getters
	std::string getRequestBuffer() const { return this->_requestBuffer; };

	// int			getClientSocket() { return clientSocket; };
	std::string getMethod() { return _method; };
	int			getNbMethod();
	std::string getUri() { return _uri; };
	std::string getProtocol() { return _protocol; };

	void checkBytesInFile();
};

#endif