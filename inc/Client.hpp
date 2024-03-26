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
	std::string 						_requestBuffer;
	std::string 						_boundary;
	std::string							m_cgiBody;
	bool								_isDir;
	CgiToServer							m_cgiToServer;
	ServerToCgi							m_serverToCgi;
	std::map <std::string, std::string> _error_pages;
	std::map <std::string, Location>	_location;
	std::string							_query;
	std::string							_path;
	long long							_maxBodySize;
	std::string						    _file_if_dir;

	std::string 						_method;
	std::string 						_uri;
	std::string 						_protocol;
	std::map <std::string, std::string> headerMap;

	std::string 						_body;
	long long							_contentLength;
	std::string 						_fileNameBody;
	bool								_isDelete;
	std::string 						_contentType;

    struct sockaddr_storage 			m_client_address {};
    socklen_t 							m_addrlen{sizeof(m_client_address)};

	std::string							_pytyhonScript;
	std::string							_pathInfo;
	std::string							_accumulatedRequestData;

public:
	Response							_response;
    Client(Server &server, std::map<std::string, std::string> ErrorPages, std::map<std::string, Location> Locations );
    ~Client();
    Client &operator=(Client const &copy);

	// REQUEST AND RESPONSE 
	void		receiveRequest();
	int			readBuffer();
	bool		checkPathAndMethod();
	bool		allowedMethods();
	void		handleResponse();

	// EPOLL
	void		removeFromEpoll(int fd);
	void		modifyEpoll(Socket *ptr, int events, int fd);
	void		addCGIProcessToEpoll(Socket *ptr, int events, int fd);

	// PARSER
	bool		checkMethod(std::string tmp);
	void		handleRequest(std::string request);
	bool		checkRequestLine(std::string httpRequest);
	int			parseRequest(std::string request);
	bool		isRequestComplete(std::string accumulatedRequestData);
	void		checkBytesInFile();
	bool 		checkForSpaces(std::string fileNameBody);
	std::string urlEncode(const std::string& input);
	std::string decodePercentEncoding(const std::string &encoded);
	int			transferData();
	void		checkBoundary();

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

	void		openAndClose();
};

// #endif