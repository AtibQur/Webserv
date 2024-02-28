#include "main.hpp"

void Client::modifyEpoll(Socket *ptr, int events, int fd)
{
    struct epoll_event event;
    event.events = events;
    event.data.ptr = ptr;

    if (epoll_ctl(m_epoll, EPOLL_CTL_MOD, fd, &event) == -1)
    {
        Response EpollError(getSocketFd(), "500 Internal Server Error");
        _response = EpollError;
    }
}

void Client::removeFromEpoll(int fd){
    modifyEpoll(nullptr, 0, fd);
    if (epoll_ctl(m_epoll, EPOLL_CTL_DEL, fd, NULL) == -1)
        perror ("remove epoll");
}

void Client::addCGIProcessToEpoll(Socket *ptr, int events, int fd)
{
    struct epoll_event event;
    event.events = events;
    event.data.ptr = ptr;

    if (epoll_ctl(this->m_epoll, EPOLL_CTL_ADD, fd, &event) == -1)
    {
        throw (std::invalid_argument("500 cgi epoll Error"));
    }
}
