#include "../../inc/main.hpp"

bool Server::isRequestComplete(std::string accumulatedRequestData){
    ssize_t requestEnd;
    requestEnd = accumulatedRequestData.find("\r\n\r\n");
    if (requestEnd == std::string::npos){
        std::cout << "the request is not complete" << std::endl;
        return false;
    }
    else {
        std::cout << "the request is complete" << std::endl;
        return true;
    }
}
