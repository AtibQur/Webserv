#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "main.hpp"

class Location;
class Server;

class Client : public Socket {
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

    struct sockaddr_storage m_client_address {};
    socklen_t m_addrlen{sizeof(m_client_address)};

	std::string _requestBuffer;
public:
    Client();
    Client(const Server &server, std::map<std::string, std::string> ErrorPages, std::map<std::string, Location> Locations );
    ~Client();
    Client(Client const &copy);
    Client &operator=(Client const &copy);

	void	readBuffer();
	void	createErrorResponse(const std::string& errorMessage);
	void	modifyEpoll(Socket *ptr);

	// parser
    void	saveClientRequest(char* buffer, int client_socket);
	bool	checkMethod(std::string tmp);
	void	handleRequest(Server *server, std::string request, char *buffer, ssize_t post);
	bool	checkRequestLine(std::string httpRequest);
	int		parseRequest(std::string request, char* buffer, ssize_t post);

	bool isRequestComplete(std::string accumulatedRequestData, ssize_t post);
	int getSocketFd() const { return this->m_socketFd; };

	// getters
	std::string getRequestBuffer() const { return this->_requestBuffer; };

	void setEpoll(int newEpoll) { m_epoll = newEpoll; };
	// int			getClientSocket() { return clientSocket; };
	std::string getMethod() { return _method; };
	int			getNbMethod();
	std::string getUri() { return _uri; };
	std::string getProtocol() { return _protocol; };

	void checkBytesInFile();
};

#endif