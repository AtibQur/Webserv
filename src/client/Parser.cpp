#include "../../inc/main.hpp"

/* Parse the client request */

int Client::     parseRequest(std::string request, char* buffer) {
    std::stringstream httpRequest(request);
    std::string tmp;

    // std::cout << "the request: " << request << std::endl;
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

    // response zin eindigt met /r/n
    // hele response eidigt met /r/n/r/n
    // content length bepaalt of the body compleet is (als er een body is) 

    return (0);
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
