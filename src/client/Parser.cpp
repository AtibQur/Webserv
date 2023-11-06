#include "../../inc/main.hpp"

/* Parse the client request */

int Client::parseRequest(std::string request, char* buffer) {
    std::stringstream httpRequest(request);
    std::string tmp;

    // check if there is valid request line
    if (!checkRequestLine(request)){
        throw std::invalid_argument("400 Bad Request");
    }
    getline(httpRequest, tmp, ' ');
    if (!checkMethod(tmp)){
        throw std::invalid_argument("405 Method Not Allowed");
    }
    _method = tmp;
    getline (httpRequest, tmp, ' ');

    if (tmp.empty())
        throw std::invalid_argument("400 Bad Request");
    _uri = tmp;
    getline (httpRequest, tmp);
    
    if (tmp.compare("HTTP/1.1\r")) // \r\n
        throw std::invalid_argument("400 Bad Request: Only protocol HTTP/1.1 is allowed");
    _protocol = tmp;

    std::cout << "the request line is valid" << std::endl;
    // start header
    if (request.find("\r\n\r\n") == std::string::npos){
        std::cout << "the request is not complete" << std::endl;
        return 1;
    }
    // parse header
    // std::cout << "the rest:" << request << std::endl;
    return (0);

    // response zin eindigt met /r/n
    // hele response eidigt met /r/n/r/n
    // content length bepaalt of the body compleet is (als er een body is) 


    // std::size_t contentDispositionPos = httpRequest.find("Content-Disposition");
    // if (contentDispositionPos != std::string::npos)
    //     std::cout << "FOUND!" << std::endl;

    // size_t nl = httpRequest.find("\r\n");
    // if (nl == std::string::npos)
    //     perror("400: invalid request: can't find the request line");
    // std::string requestLine = httpRequest.substr(0, nl);
    // size_t methodEnd = requestLine.find(' ');
    // if (methodEnd == std::string::npos)
    //     perror("400: can't find the end of the method");
    // size_t uriEnd = requestLine.find(' ', methodEnd + 1);
    // if (uriEnd == std::string::npos)
    //     perror("400: can't find the end of the uri");
    
    // // parse request line
    // this->_method = requestLine.substr(0, methodEnd);
    // this->_uri = requestLine.substr(methodEnd + 1, uriEnd - methodEnd - 1);
    // this->_protocol = requestLine.substr(uriEnd + 1);

    // // parse headers
    // size_t startHeaderSection = httpRequest.find("\n");
    // if (startHeaderSection == std::string::npos)
    //     perror("400: can't find the headersection line");
    // std::string headerSection = httpRequest.substr(startHeaderSection + 1);

    // size_t pos = 0;
    // while (pos < headerSection.size()){
    //     size_t lineEnd = headerSection.find("\n", pos);
    //     if (lineEnd == std::string::npos)
    //         perror("400: can't find the end of the line");
    //     std::string line = headerSection.substr(pos, lineEnd - pos);

    //     size_t seperatorPos = line.find(":");
    //     if (seperatorPos != std::string::npos){
    //         std::string key = line.substr(0, seperatorPos);
    //         std::string value = line.substr(seperatorPos + 2);
    //         headerMap[key] = value;
    //     } else {
    //         perror("400: can't find the seperator");
    //     }
    //     pos = lineEnd + 1;
    // }

    // // request body
    // std::string requestBody;
    // if (headerMap.find("Content-Type") != headerMap.end()){
    //     requestBody = headerMap["Content-Type"];
    //     if (requestBody.find("boundary=") != std::string::npos){
    //         this->_boundary = requestBody.substr(requestBody.find('=') + 1);
    //     }
    // }

    // // print request line
    // std::cout << _method << " " << _uri << " " << _protocol << std::endl;
    // // print map
    // std::map<std::string, std::string>::iterator it = headerMap.begin();
    // while (it != headerMap.end())
    // {
    //     std::cout << "-> " << it->first << " -> " << it->second << std::endl;
    //     ++it;
    // }
    // std::cout << "b --> " << this->_boundary << std::endl;
    // return 0;
}

// check if the requestline is valid
bool Client::checkRequestLine(std::string httpRequest)
{
    size_t space1 = httpRequest.find(' ');
    size_t space2 = httpRequest.find(' ', space1 + 1);

    if (space1 == std::string::npos || space2 == std::string::npos)
        return false; 
    _method = httpRequest.substr(0, space1);
    _uri = httpRequest.substr(space1 + 1, space2 - space1 - 1);
    _protocol = httpRequest.substr(space2 + 1);
    return true;
}

// check if the method is valid
bool Client::checkMethod(std::string tmp){
    static const std::string validMethods[3] = {"GET", "POST", "DELETE"};
    for (int i = 0; i < 3; i++){
        if (tmp == validMethods[i])
            return true;
    }
    return false;
}

// void Client::parseRequest(char* buffer) {
//     std::string httpRequest;
//     std::istringstream ss(buffer);
//     getline(ss, httpRequest);

//     // std::size_t contentDispositionPos = httpRequest.find("Content-Disposition");
//     // if (contentDispositionPos != std::string::npos)
//     //     std::cout << "FOUND!" << std::endl;

//     std::string requestLine = httpRequest.substr(0);
//     size_t methodEnd = requestLine.find(' ');
//     if (methodEnd == std::string::npos)
//         perror("400: can't find the end of the method");
//     size_t uriEnd = requestLine.find(' ', methodEnd + 1);
//     if (uriEnd == std::string::npos)
//         perror("400: can't find the end of the uri");

//     // parse request line
//     this->_method = requestLine.substr(0, methodEnd);
//     this->_uri = requestLine.substr(methodEnd + 1, uriEnd - methodEnd - 1);
//     this->_protocol = requestLine.substr(uriEnd + 1);

//     // parse headers
//     size_t pos = 0;
//     while (getline(ss, httpRequest) && httpRequest[0] != '\r')
//     {
//         size_t index = httpRequest.find(':', 0);
//         if (index == std::string::npos){
//             perror("400: can't find the seperator");
//             break ;
//         }
//         std::string key = httpRequest.substr(0, index);
//         std::string value = httpRequest.substr(index + 2);
//         headerMap[key] = value;
//     }

//     // // request body
//     // std::string requestBody;
//     // if (headerMap.find("Content-Type") != headerMap.end()){
//     //     requestBody = headerMap["Content-Type"];
//     //     if (requestBody.find("boundary=") != std::string::npos){
//     //         this->_boundary = requestBody.substr(requestBody.find('=') + 1);
//     //     }
//     // }

//     // // print request line
//     std::cout << "request line: " << _method << " " << _uri << " " << _protocol << std::endl;
//     // print map
//     std::map<std::string, std::string>::iterator it = headerMap.begin();
//     while (it != headerMap.end())
//     {
//         std::cout << "-> " << it->first << " -> " << it->second << std::endl;
//         ++it;
//     }
//     // std::cout << "b --> " << this->_boundary << std::endl;
// }