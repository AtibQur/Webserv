#ifndef SOCKET_HPP
#define SOCKET_HPP

class Socket
{
public:
    int m_socketFd{};
    int m_epoll;
    virtual ~Socket(void) = 0;
};

#endif