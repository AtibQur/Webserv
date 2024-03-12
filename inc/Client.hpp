// #ifndef CLIENT_HPP
// #define CLIENT_HPP
#pragma once

#include "main.hpp"

class Location;
class Server;
class cgiToServer;

class Client : public Socket {
private:
	const Server						 &m_server;
	// Response							_response;

	CgiToServer							m_cgiToServer;
	ServerToCgi							m_serverToCgi;

	std::string 						_method;
	std::string 						_uri;
	std::string 						_protocol;
	std::map <std::string, std::string> headerMap;

	std::string 						_body;
	std::string 						_boundary;
	long long							_contentLength;
	std::string 						_fileNameBody;
	bool								_isDelete;
	bool								_isDir;
	std::string 						_contentType;
	std::map <std::string, std::string> _error_pages;
	long long							_maxBodySize;
	std::map <std::string, Location>	_location;

    struct sockaddr_storage 			m_client_address {};
    socklen_t 							m_addrlen{sizeof(m_client_address)};
	std::string 						_requestBuffer;
	std::string							m_cgiBody;
	std::string						    _file_if_dir;

	std::string							_query;
	std::string							_path;
	std::string							_pytyhonScript;

public:
	Response							_response;
    Client();
    Client(Server &server, std::map<std::string, std::string> ErrorPages, std::map<std::string, Location> Locations );
    ~Client();
    Client &operator=(Client const &copy);

	// REQUEST AND RESPONSE 
	void		receiveRequest();
	void		readBuffer();
	bool		checkPathAndMethod();
	bool		allowedMethods();
	void		createErrorResponse(const std::string& errorMessage);
	void		handleResponse();

	// EPOLL
	void		removeFromEpoll(int fd);
	void		modifyEpoll(Socket *ptr, int events, int fd);
	void		addCGIProcessToEpoll(Socket *ptr, int events, int fd);

	// PARSER
    void		saveClientRequest(char* buffer, int client_socket);
	bool		checkMethod(std::string tmp);
	void		handleRequest(std::string request, ssize_t post);
	bool		checkRequestLine(std::string httpRequest);
	int			parseRequest(std::string request, ssize_t post);
	bool		isRequestComplete(std::string accumulatedRequestData, ssize_t post);
	void		checkBytesInFile();
	bool 		checkForSpaces(std::string fileNameBody);
	std::string urlEncode(const std::string& input);
	std::string decodePercentEncoding(const std::string &encoded);
	int			transferData();
	int 		checkBoundary(std::string contentType);

	// GETTERS
	int			getSocketFd() const { return this->m_socketFd; };
	std::string getRequestBuffer() const { return this->_requestBuffer; };
	std::string getMethod() { return _method; };
	int			getNbMethod();
	std::string getUri() { return _uri; };
	std::string getProtocol() { return _protocol; };
	std::string getFileNameBody() { return _fileNameBody; };
	std::string getBody() { return _body; };
	Response	getResponse() { return _response; };
	CgiToServer	&getcgiToServer() { return m_cgiToServer; };
	std::string getCgiBody () { return m_cgiBody; };

	// SETTERS
	void		setUri(std::string uri);
	void		setEpoll(int newEpoll) { m_epoll = newEpoll; };

	char		hexToChar(const std::string &hex);
	void		handleGetMethod();
	std::string generateDirectoryListing(std::string dirPath);
	void		handlePostMethod();
	void		handleDeleteMethod();
	void		createErrorResponse();

	void		handleCGI();
	int			execute();
	void		setError(int socket, std::string message);
	void		extractcgiUri();

	// SIGNAL
	static void		handleSignal(int signum);
};

// #endif