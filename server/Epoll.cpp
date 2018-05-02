#include "Epoll.h"

#include <iostream>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <typeinfo>
#include <exception>

Epoll::Epoll(unsigned int max_events):
    m_max_events(max_events)
{
    m_epollfd = epoll_create(m_max_events);
    m_events = new epoll_event[m_max_events];   
}

Epoll::~Epoll()
{
    delete[] m_events;
    close(m_epollfd);
}

void Epoll::addEvent(int fd)
{
    m_event.events = EPOLLIN | EPOLLET;
    m_event.data.fd = fd;
    if (epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &m_event) == -1)
    {
       std::cout << "failed epoll_ctl" << std::endl;
       throw(strerror(errno));
    }
}

int Epoll::wait() const
{
    std::cout << "Waiting connection..." << std::endl;;
    int countActivefd = epoll_wait(m_epollfd, m_events, m_max_events, -1);
    if (countActivefd <= -1)
    {
        std::cout << "epoll_wait failed" << std::endl;
        throw(strerror(errno));
    }

    return countActivefd;

}

int Epoll::getfd(const int index) const
{
    return m_events[index].data.fd;
}

int Epoll::getEpollfd() const
{
    return m_epollfd;
}
