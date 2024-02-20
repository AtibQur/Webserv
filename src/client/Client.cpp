#include "Client.hpp"

Client::Client() : m_server(nullptr), _requestBuffer(""), _boundary("UNSET"), m_name(""), _isDir(false), m_cgiToServer(*this), m_serverToCgi(*this)
{
    m_socketFd = -1;
    _query = "";
    _path = "";
}

Client::Client(Server &server, std::map<std::string, std::string> ErrorPages, std::map<std::string, Location> Locations)
    : m_server(server), _boundary("UNSET"), m_cgiToServer(*this), m_serverToCgi(*this)
{
    _error_pages = ErrorPages;
    _location = Locations;
    _query = "";
    _isDir = false;
    _path = "";
    _maxBodySize = server.getConf()->getMaxBodySize();
    _file_if_dir = server.getConf()->getFileIfDir();

    m_socketFd = accept(server.getSockFd(), (struct sockaddr *)&m_client_address, &m_addrlen);
    if (m_socketFd == -1)
    {
        setError(m_socketFd, "500 Internal Server Error");
    }
}

Client::~Client()
{
    std::cout << "Client removed" << std::endl;
}

// Client::Client(Client const &copy) {
//     *this = copy;
// }

Client &Client::operator=(Client const &copy)
{
    this->m_socketFd = copy.m_socketFd;
    this->_requestBuffer = copy._requestBuffer;
    this->_response = copy._response;
    std::cout << "client operator construcotr called " << std::endl;
    return *this;
}

int Client::getNbMethod()
{
    if (_method == "GET")
        return 1;
    if (_method == "POST")
        return 2;
    if (_method == "DELETE")
        return 3;
    return (0);
}

void Client::modifyEpoll(Socket *ptr, int events, int fd)
{
    struct epoll_event event;
    event.events = events;

    event.data.ptr = ptr;
    if (epoll_ctl(m_epoll, EPOLL_CTL_MOD, fd, &event) == -1)
    {
        std::cerr << "Error modifying epoll" << std::endl;
        setError(m_socketFd, "500 Internal Server Error");
    }
}

void Client::removeFromEpoll(int fd){
    modifyEpoll(nullptr, 0, fd);
    if (epoll_ctl(m_epoll, EPOLL_CTL_DEL, fd, NULL) == -1)
        perror ("remove epoll");
}

void Client::receiveRequest()
{
    try
    {
        readBuffer();
    }
    catch (const std::exception &e)
    {
        setError(getSocketFd(), e.what());
    }
}

void Client::handleRequest(std::string request, ssize_t post)
{
    try
    {
        parseRequest(request, post);
        checkPathAndMethod();

    }
    catch (const std::exception &e)
    {
        setError(getSocketFd(), e.what());
    }
}

void Client::readBuffer()
{
    ssize_t post = 0;
    ssize_t i = 0;
    char buffer[1024] = {0};
    ssize_t bytes_read;
    std::string accumulatedRequestData;
    std::string hardcodedrequest;

    while (1)
    {
        bytes_read = read(getSocketFd(), buffer, sizeof(buffer));
        if (bytes_read < 0)
        {
            std::cerr << "Error reading from client socket" << std::endl;
            throw(std::invalid_argument("400 Bad Request"));
            close(getSocketFd());
            break;
        }
        else if (bytes_read == 0)
        {
            std::cout << "Connection closed by the client." << std::endl;
            close(getSocketFd());
            break;
        }
        else if (i = 0)
        {
            if (buffer[0] == 'P' && buffer[1] == 'O' && buffer[2] == 'S' && buffer[3] == 'T' && buffer[4] == ' ')
                post = 1;
        }
        else
        {
            accumulatedRequestData.append(buffer, bytes_read);
            if (bytes_read == 1024)
                continue;
            if (isRequestComplete(accumulatedRequestData, post))
            {
                modifyEpoll(this, EPOLLOUT, getSocketFd());
                handleRequest(accumulatedRequestData, post);
                
                break;
            }

        }
        i++;
    }
}

bool Client::isRequestComplete(std::string accumulatedRequestData, ssize_t post)
{
    ssize_t requestEnd;
    std::string boundary;
    ssize_t startOfBoundary = accumulatedRequestData.find("boundary=");
    if (post)
        requestEnd = accumulatedRequestData.find("\r\n\r\n");
    else
        requestEnd = accumulatedRequestData.find("\r\n\r\n");
    if (requestEnd == std::string::npos)
    {
        std::cout << "the request is not complete" << std::endl;
        return false;
    }
    else
    {
        return true;
    }
}

void Client::setError(int socket, std::string message)
{
    Response errorResponse(socket, message);
    _response = errorResponse;
}

/* CREATE ERROR RESPONSE */
void Client::createErrorResponse()
{
    std::string file;
    std::string response;

    file = m_server.getConf()->getErrorPage(_response.getHeader());
    std::cout << "Error code: " << _response.getHeader() << std::endl;
    std::ifstream htmlFile(file);

    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));
    htmlFile.close();

    _response.setErrorResponse("HTTP/1.1 " + _response.getCode() + "\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);

    _response.sendResponse();
}
