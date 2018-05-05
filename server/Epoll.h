#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>

class Epoll
{
public:
    explicit Epoll(unsigned int max_events);
    ~Epoll();
    void addEvent(int fd);
    int wait() const;
    int getfd(int index) const;
    int getEpollfd() const;
private:
    int m_epollfd;
    unsigned int m_max_events;    
    epoll_event* m_events;
};

#endif
