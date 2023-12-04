#ifndef SOCKET_HPP
#define SOCKET_HPP

class Socket
{
protected:
    int m_socketFd{};
    int m_epoll;
public:
    virtual ~Socket(void) = 0;
};

#endif