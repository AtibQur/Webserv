#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "main.hpp"

class Location;
class Server;

class Client : public Socket {
private:
	const Server						 &m_server;
	Response							_response;

	std::string 						_method;
	std::string 						_uri;
	std::string 						_protocol;
	std::map <std::string, std::string> headerMap;

	std::string 						_body;
	std::string 						_boundary;
	long long							_contentLength;
	std::string 						_fileNameBody;
	std::string 						_contentType;
	std::map <std::string, std::string> _error_pages;
	std::map <std::string, Location>	_location;

    struct sockaddr_storage 			m_client_address {};
    socklen_t 							m_addrlen{sizeof(m_client_address)};
	std::string 						_requestBuffer;

public:
    Client();
    Client(Server &server, std::map<std::string, std::string> ErrorPages, std::map<std::string, Location> Locations );
    ~Client();
    Client &operator=(Client const &copy);

	// REQUEST AND RESPONSE 
	void		readBuffer();
	bool		isPathAndMethodAllowed();
	void		modifyEpoll(Socket *ptr, int events, int fd);
	void		createErrorResponse(const std::string& errorMessage);
	void		sendResponse();

	// PARSER
    void		saveClientRequest(char* buffer, int client_socket);
	bool		checkMethod(std::string tmp);
	void		handleRequest(std::string request, char *buffer, ssize_t post);
	bool		checkRequestLine(std::string httpRequest);
	int			parseRequest(std::string request, char* buffer, ssize_t post);
	bool		isRequestComplete(std::string accumulatedRequestData, ssize_t post);
	void		checkBytesInFile();

	// GETTERS
	int			getSocketFd() const { return this->m_socketFd; };
	std::string getRequestBuffer() const { return this->_requestBuffer; };
	std::string getMethod() { return _method; };
	int			getNbMethod();
	std::string getUri() { return _uri; };
	std::string getProtocol() { return _protocol; };
	std::string getFileNameBody() { return _fileNameBody; };

	// SETTERS
	void		setEpoll(int newEpoll) { m_epoll = newEpoll; };

};

#endif