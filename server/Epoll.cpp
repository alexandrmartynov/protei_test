#include "Epoll.h"

#include <iostream>
#include <fcntl.h>

Epoll::Epoll(unsigned int max_events):
    m_epollfd(0),
    m_max_events(max_events)
{}

Epoll::~Epoll()
{
    delete[] m_events;
}

void Epoll::createEpollfd()
{
    m_epollfd = epoll_create(m_max_events);
}

void Epoll::createEvents()
{
    m_events = new epoll_event[m_max_events];
}

void Epoll::addEvent(int fd)
{
    m_event.events = EPOLLIN | EPOLLET;
    m_event.data.fd = fd;
    if (epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &m_event) == -1)
    {
       std::cout << "failed epoll_ctl" << std::endl;
       return;
    }
}

/*int Epoll::setNonBlockingSocket(int& socket) const
{
    int result = -1;
    int flag = fcntl(socket, F_GETFL);
    
    if(flag >= 0)
    {

        flag = (flag | O_NONBLOCK);
        if(fcntl(socket, F_SETFL, flag) >= 0)
        {
            result = 0;
            std::cout << "Set flag to file descriptor" << std::endl;
        }
    }

    return result;
}*/

int Epoll::wait() const
{
    std::cout << "Waiting connection..." << std::endl;;
    int nfds = epoll_wait(m_epollfd, m_events, m_max_events, -1);
    if (nfds <= -1)
    {
        std::cout << "epoll_wait failed" << std::endl;
    }

    return nfds;

}

int Epoll::getfd(const int index) const
{
    return m_events[index].data.fd;
}

int Epoll::getEpollfd() const
{
    return m_epollfd;
}
