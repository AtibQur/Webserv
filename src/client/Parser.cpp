#include "../../inc/main.hpp"

/* Parse the client request */

std::string subTillChar(std::string str, size_t index, char c) {
    std::string tmp = "";
    size_t pos1 = str.find('\r', index);
    size_t pos2 = str.find(c, index);
    if (pos1 != std::string::npos || pos2 != std::string::npos) {
        if (pos1 < pos2) {
            tmp = str.substr(index, pos1 - index);
        }
        else {
            tmp = str.substr(index, pos2 - index);
        }
    }
    return tmp;
}

void Client::checkBytesInFile() {
    std::ifstream in_file("root/body.txt", std::ios::binary);
    in_file.seekg(0, std::ios::end);
    int file_size = in_file.tellg();
    std::cout<<"Size of the file is"<<" "<< file_size <<" "<<"bytes";
}

void ptn(std::string str) {
    std::cout << str << std::endl;
    int i = 0;
    while (str[i]) {
        if (str[i] == '\r')
            std::cout << "r";
        else if (str[i] == '\n')
            std::cout << "n";
        else
            std::cout << str[i];
        i++;
        break ;
    }
}

int Client::parseRequest(std::string request, char* buffer, ssize_t post) {
    std::stringstream httpRequest(request);
    std::string tmp;

    // check if there is valid request line
    if (!checkRequestLine(request)){
        throw std::invalid_argument("400");
    }
    getline(httpRequest, tmp, ' ');
    if (!checkMethod(tmp)){
        throw std::invalid_argument("405");
    }
    _method = tmp; 
    getline (httpRequest, tmp, ' ');

    if (tmp.empty())
        throw std::invalid_argument("400");
    _uri = tmp;
    getline (httpRequest, tmp);
    
    if (tmp.compare("HTTP/1.1\r")) // \r\n
        throw std::invalid_argument("400");
    _protocol = tmp;

    // start header

    if (request.find("\r\n\r\n") == std::string::npos){
        std::cout << "the request is not complete" << std::endl;
        return 1;
    } else if (_method == "GET"){
        return 0;
    }
    // parse header
    while (getline(httpRequest, tmp)) {
        if (tmp.find("--" + _boundary) != std::string::npos)
            break ;
        if (tmp.find("Content-Type:") != std::string::npos) {
            _contentType = subTillChar(tmp, tmp.find("Content-Type:") + 14, ';');
            std::cout << "^" << _contentType << "^" << std::endl;
            _boundary = tmp.substr(tmp.find("boundary") + 9);
        }
        if (tmp.find("Content-Length:") != std::string::npos) {
            _contentLength = stoll(tmp.substr(tmp.find("Content-Length:") + 16));
        }
    }
    if (_contentLength == 0)
        throw std::invalid_argument("400 Bad Request: Content-Length is 0");
    if (_contentType.empty())
        throw std::invalid_argument("400 Bad Request: Content-Type is empty");
    if (_boundary.empty())
        throw std::invalid_argument("400 Bad Request: Boundary is empty");
    if (_contentLength > 1000000) // needs to be updated from conf file
        throw std::invalid_argument("413 Payload Too Large: Content-Length is too large");
    if (_contentType != "multipart/form-data")
        return (0); // for when its text or www-form-urlencoded

    // parse body
    getline(httpRequest, tmp);
    if (tmp.find("filename=") != std::string::npos)
        _fileNameBody = subTillChar(tmp, tmp.find("filename=") + 10, '\"');
    getline(httpRequest, tmp);
    if (tmp.find("Content-Type:") != std::string::npos)
        _contentType = subTillChar(tmp, tmp.find("Content-Type:") + 14, '\r');
    getline(httpRequest, tmp);

    while (getline(httpRequest, tmp)) {
        if (tmp.find(_boundary + "--") != std::string::npos)
            break ;
        _body.append(tmp);
        _body.append("\n");
    }

    std::stringstream ss(_body);
    std::string read;
    std::ofstream bodyfile;
    // // // parse body

    bodyfile.open ("./root/" + _fileNameBody);
    while (getline(ss, read, '\n')) {
			if (read.compare("--" + _boundary + "--") == 0)
				break;
			bodyfile << read;
			bodyfile << std::endl;
	}
    bodyfile.close();

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
