#include "Client.hpp"

namespace fs = std::filesystem;

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
    _accumulatedRequestData = "";
    _contentLength = 0;

    m_socketFd = accept(server.getSockFd(), (struct sockaddr *)&m_client_address, &m_addrlen);
    if (m_socketFd == -1)
    {
        setError(m_socketFd, "500 Internal Server Error");
    }
}

Client::~Client()
{
    close(m_socketFd);
}

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

void Client::receiveRequest()
{
    try
    {
        if (readBuffer() == 1)
        {
            return;
        }
    }
    catch (const std::exception &e)
    {
        setError(getSocketFd(), e.what());
    }
}

void Client::handleRequest(std::string request)
{
    try
    {
        parseRequest(request);
        checkPathAndMethod();
    }
    catch (const std::exception &e)
    {
        setError(getSocketFd(), e.what());
    }
}

int Client::readBuffer()
{
    char buffer[1024] = {0};
    ssize_t bytes_read;

    bytes_read = read(getSocketFd(), buffer, sizeof(buffer));
    std::string tmp(buffer);
    if (tmp.find("Content-Length: ") != std::string::npos)
    {
        _contentLength = stoll(tmp.substr(tmp.find("Content-Length:") + 16));
    }
    if (bytes_read < 0)
    {
        close(getSocketFd());
        delete this;
        throw(std::invalid_argument("400 Bad Request"));
    }
    else if (bytes_read == 0)
    {
        close(getSocketFd());
        std::cout << "client closed" << std::endl;
    }
    else
    {
        _accumulatedRequestData.append(buffer, bytes_read);
        if (isRequestComplete(_accumulatedRequestData) == false)
        {
            return 1;
        }
        else
        {
            handleRequest(_accumulatedRequestData);
        }
    }
    return 0;
}

bool Client::isRequestComplete(std::string accumulatedRequestData)
{
    size_t requestEnd;
    requestEnd = accumulatedRequestData.find("\r\n\r\n");
    if (requestEnd == std::string::npos)
    {
        return false;
    }
    if (_contentLength != 0) // if post request
    {
        if (accumulatedRequestData.size() < _contentLength + requestEnd + 4)
        {
            return false;
        }
        else
        {
            std::cout << "Request complete" << std::endl;
            return true;
        }
    }
    else
    {
        return true;
    }
}

int checkCgiPath(std::string path)
{
    size_t pos = path.find(".py");

    if (pos != std::string::npos)
    {
        if ((pos + 3) != path.size())
        {
            if (path[pos + 3] != '/' && path[pos + 3] != '?')
                return 0;
            else
                return 1;
        }
        return 1;
    }
    return 0;
}

/* REQUEST CHECK IF THE URI IS IN LOCATIONS AND IF THE METHOD IS ALLOWED*/
bool Client::checkPathAndMethod()
{
    Location clientLocation = m_server.getConf()->getLocation(getUri());
    std::string root_folder = m_server.getConf()->getRoot();

    /* CGI */
    if (checkCgiPath(getUri()))
    {
        if (getMethod() == "GET")
        {
            try
            {
                addCGIProcessToEpoll(&m_cgiToServer, EPOLLIN, m_cgiToServer.m_pipeFd[READ]); // add PipeOut to epoll
                m_cgiToServer.m_client.handleCGI();
            }
            catch (const std::exception &e)
            {
                Response cgiError(getSocketFd(), e.what());
                _response = cgiError;
                modifyEpoll(this, EPOLLOUT, getSocketFd());
                return true;
            }
        }
        else if (getMethod() == "POST")
        {
            addCGIProcessToEpoll(&m_serverToCgi, EPOLLOUT, m_serverToCgi.m_pipeFd[WRITE]); // add write end to pipeIn to epoll
        }
        return true;
    }
    modifyEpoll(this, EPOLLOUT, getSocketFd());

    if (getUri() == "/teapot")
    {
        throw std::invalid_argument("418 I'm a teapot");
    }
    if (!fs::exists(root_folder + getUri()))
    {
        throw std::invalid_argument("404 Not Found");
    }
    if (fs::is_regular_file(root_folder + getUri()))
        return true;
    if (fs::is_directory(root_folder + getUri()) && clientLocation.getPath() != getUri())
    {
        _isDir = true;
        if (_file_if_dir.empty())
        {
            setError(getSocketFd(), "404");
            createErrorResponse();
        }
    }
    if (clientLocation.getIndex().empty() && clientLocation.getAutoIndex() == false)
    {
        throw std::invalid_argument("404 Not Found");
    }
    if (clientLocation.getPath().empty())
    {
        throw std::invalid_argument("404 Not Found");
    }
    if ("/root/" + access(getUri().c_str(), R_OK) == 0)
    {
        return true;
    }

    std::vector<std::string> methods = clientLocation.getMethods();
    if (methods.empty())
    {
        throw std::invalid_argument("405 Method Not Allowed");
    }
    for (std::string method : methods)
    {
        if (method == getMethod())
        {
            return true;
        }
    }
    throw std::invalid_argument("405 Method Not Allowed");
}

void Client::handleResponse()
{
    if (_response.getHeader().empty())
    {
        int method = getNbMethod();
        switch (method)
        {
        case 1:
            handleGetMethod();
            break;
        case 2:
            handlePostMethod();
            break;
        case 3:
            handleDeleteMethod();
            break;
        default:
            std::cout << "Default Method" << std::endl;
        }
    }
    else
    {
        std::cout << "Creating Error Response" << std::endl;
        createErrorResponse();
    }
    modifyEpoll(this, EPOLLIN, getSocketFd());
    _response = Response();
}

void Client::setError(int socket, std::string message)
{
    modifyEpoll(this, EPOLLOUT, getSocketFd()); // client error epollout
    Response errorResponse(socket, message);
    _response = errorResponse;
}

/* CREATE ERROR RESPONSE */
void Client::createErrorResponse()
{
    std::string file;
    std::string response;

    file = m_server.getConf()->getErrorPage(_response.getCode());
    std::ifstream htmlFile(file);

    std::string fileContent((std::istreambuf_iterator<char>(htmlFile)), (std::istreambuf_iterator<char>()));
    htmlFile.close();

    _response.setErrorResponse("HTTP/1.1 " + _response.getCode() + "\nContent-Length: " + std::to_string(fileContent.size()) + "\n\n" + fileContent);

    _response.sendResponse();
    openAndClose();
}
