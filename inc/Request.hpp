// #pragma once

// #include "main.hpp"

// class Location;

// class Request 
// {
// private:
// 	std::string 						_method;
// 	std::string 						_uri;
// 	std::string 						_protocol;
// 	std::map <std::string, std::string> headerMap;

//     std::string 						_body;
// 	std::string 						_boundary;
// 	long long							_contentLength;
// 	std::string 						_fileNameBody;
// 	bool								_isDelete;
// 	bool								_isDir;
// 	std::string 						_contentType;
// 	std::map <std::string, std::string> _error_pages;
// 	long long							_maxBodySize;
// 	std::map <std::string, Location>	_location;

//     std::string							m_name;

// public:
//     void		saveClientRequest(char* buffer, int client_socket);
// 	bool		checkMethod(std::string tmp);
// 	void		handleRequest(std::string request, ssize_t post);
// 	bool		checkRequestLine(std::string httpRequest);
// 	int			parseRequest(std::string request, ssize_t post);
// 	bool		isRequestComplete(std::string accumulatedRequestData, ssize_t post);
// 	void		checkBytesInFile();
// 	bool 		checkForSpaces(std::string fileNameBody);
// 	std::string urlEncode(const std::string& input);
// 	std::string decodePercentEncoding(const std::string &encoded);
// 	int			transferData();

//     char		hexToChar(const std::string &hex);
// };