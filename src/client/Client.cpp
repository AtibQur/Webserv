#include "Client.hpp"

namespace fs = std::filesystem;

Client::Client() : m_server(nullptr), _requestBuffer(""), _boundary("UNSET"), m_cgiBody(""), _isDir(false), m_cgiToServer(*this), m_serverToCgi(*this)
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
        std::cout << "Hello from handleRequest" << std::endl;
        setError(getSocketFd(), e.what());
    }
}

void Client::handleSignal(int signum) {
    delete this;
}

void Client::readBuffer()
{
    signal(SIGINT, handleSignal);
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
            delete this;
            std::cout << "client deleted" << std::endl;
            close(getSocketFd());
            throw(std::invalid_argument("400 Bad Request"));
            break;
        }
        else if (bytes_read == 0)
        {
            close(getSocketFd());
            delete this;
            std::cout << "client deleted" << std::endl;
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

    requestEnd = accumulatedRequestData.find("\r\n\r\n");
    if (requestEnd == std::string::npos)
    {
        throw std::invalid_argument("400 Bad Request");
        return false;
    }
    else
    {
        return true;
    }
}

/* REQUEST CHECK IF THE URI IS IN LOCATIONS AND IF THE METHOD IS ALLOWED*/
bool Client::checkPathAndMethod()
{
    Location clientLocation = m_server.getConf()->getLocation(getUri());

    /* CGI */
    if (getUri().find(".py") != std::string::npos)
    {
        if (getMethod() == "GET")
        {
            try 
            {
                addCGIProcessToEpoll(&m_cgiToServer, EPOLLIN, m_cgiToServer.m_pipeFd[READ]); // add PipeOut to epoll
                m_cgiToServer.m_client.handleCGI();
            } catch (const std::exception &e) 
            {
                std::cout << "ERROR: " << e.what() << std::endl;
                Response cgiError(getSocketFd(), e.what());
                _response = cgiError;
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

    //! need to make sure to seperate this
    //! add client or cgi to epollout depening on if it's a cgi or not

    if (_method == "DELETE")
        return true;
    if (getUri() == "/teapot")
    {
        throw std::invalid_argument("418 I'm a teapot");
    }
    if (!fs::exists("root" + getUri()))
    {
        std::cout << "1" << std::endl;
        throw std::invalid_argument("404 Not Found");
    }
    if (fs::is_regular_file("root" + getUri()))
        return true;
    if (fs::is_directory("root" + getUri()) && clientLocation.getPath() != getUri())
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
        std::cout << "error after" << "\n";
        return true;
    }

    std::vector<std::string> methods = clientLocation.getMethods();
    if (methods.empty())
    {
        throw std::invalid_argument("405 Method Not Allowed");
    }
    std::vector<std::string>::iterator it = methods.begin();
    for (it; it < methods.end(); it++)
    {
        if (getMethod() == *it)
        {
            return true;
        }
    }
    if (it == methods.end())
    {
        throw std::invalid_argument("405 Method Not Allowed");
    }
    throw std::invalid_argument("400 Bad Request");
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
}

/*
Stackoverflows top answer


Alternatively, don't catch the signal and just let the OS handle the cleanup
as it's going to do during process cleanup anyway. You're not releasing any 
resources that aren't tied directly to the process
, so there's no particular need to manually release them.

*/

/*
 what were doing




    Have your program run a main processing loop.
    Have your main processing loop check a flag to see if it should "keep running".
    Have your signal handler simply set the "keep running" flag to false, but not otherwise terminate the program.
    Have your main processing loop do the memory cleanup prior to exiting.

This has the benefit of placing both the allocation and de-allocation in blocks of code 
which are called with a known sequence. Doing so can be a godsend when dealing with webs of interrelated objects, and there is not going to be race condition between two processing flows trying to mess with the same object.

*/