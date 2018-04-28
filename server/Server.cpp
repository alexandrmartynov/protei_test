#include "Server.h"

#include <iostream>
#include <fcntl.h>
#include <cstdio>
#include <sys/epoll.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <typeinfo>
#include <algorithm>

#define LOCALHOST "127.0.0.1"
#define PORT 8080

Server::Server():
    m_buffer(new char[1024])
{}

Server::~Server()
{
    delete[] m_buffer;
}

void Server::addEvent(int epollfd, int fd) const
{
    epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1)
    {
       std::cout << "failed epoll_ctl" << std::endl;
       return;
    }
}

int Server::setnonblocking(int socket)
{
    int ret = -1;
    int flag = fcntl(socket, F_GETFL);
    
    if(flag >= 0)
    {

        flag = (flag | O_NONBLOCK);
        if(fcntl(socket, F_SETFL, flag) >= 0)
        {
            ret = 0;
        }
    }

    return ret;

}

int Server::exec()
{
    setup();

    Socket_udp m_socket_udp;
    m_socket_udp.create();
    int sock_udp = m_socket_udp.getSocket();
    setnonblocking(sock_udp);
    m_socket_udp.binded(m_server_addr);

    Socket_tcp m_socket_tcp;
    m_socket_tcp.create();
    m_socket_tcp.binded(m_server_addr);
    int listen_sock = m_socket_tcp.getSocket();
    setnonblocking(listen_sock);
    m_socket_tcp.listening();

    int nfds = 0;
    int epollfd = 0;
    epoll_event ev;
    epoll_event events[MAX_EVENTS];
    epollfd = epoll_create(MAX_EVENTS);

    addEvent(epollfd, sock_udp);
    addEvent(epollfd, listen_sock);
    
    while(true)
    {
        m_socket_tcp.setSocket(listen_sock);
        std::cout << "Waiting connection...\n";
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds <= -1)
        {
            std::cout << "epoll_wait failed" << std::endl;
            return EXIT_FAILURE;
        }

        int sock_tcp = 0;
        std::string message = {};
        for (int n = 0; n < nfds; ++n)
        {
            if(events[n].data.fd == listen_sock)
            {
                sock_tcp = m_socket_tcp.accepted(&m_client_addr);
                std::cout << "new socket tcp: " << sock_tcp << std::endl;
                if(sock_tcp < 0)
                {
                    printf("accept failed %d\n", errno);
                    return EXIT_FAILURE;
                }

                setnonblocking(sock_tcp);
        
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = sock_tcp;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock_tcp, &ev) == -1)
                {
                    std::cout << "failed epoll_ctl" << std::endl;
                    return EXIT_FAILURE;
                }
            }
            else if(events[n].data.fd == sock_udp)
            {
                std::cout << "UDP\n";
                m_socket_udp.setSocket(events[n].data.fd);
                message.clear();
                message = m_socket_udp.echo_message(m_client_addr);
                result(message);
            }
            else
            {
                std::cout << "TCP\n";
                m_socket_tcp.setSocket(events[n].data.fd);
                message.clear();
                message = m_socket_tcp.echo_message();
                result(message);
            }
        }
    }

    close(epollfd);

    return 0;

}

void Server::setup()
{
    socklen_t server_addrlen = sizeof(m_server_addr);
    std::memset(reinterpret_cast<char*>(&m_server_addr), 0 , server_addrlen);
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = INADDR_ANY;
    m_server_addr.sin_port = htons(PORT);
    int convert = inet_aton(LOCALHOST, reinterpret_cast<in_addr*>(&m_server_addr.sin_addr.s_addr));
    if(convert == 0)
    {
        std::cout << "inet_aton() failed with " << strerror(errno) << std::endl;
        return;
    }   
}

void Server::result(std::string& message) const
{
    std::list<int> numbers;
    numbers.clear();

    std::string input(message);

    parse(message, numbers);

    displayList(numbers);

    int sum_numbers = std::accumulate(numbers.begin(), numbers.end(), 0);

    std::cout << "sum of numbers: " << sum_numbers << std::endl;

    numbers.sort(std::greater<int>());

    displayList(numbers);

    auto minmax = std::minmax_element(numbers.begin(), numbers.end());
    std::cout << "min: " << *minmax.first << " max: " << *minmax.second << std::endl;
}

void Server::parse(std::string& message, std::list<int> &lst) const
{
    std::string::iterator parser = message.begin();
    while(parser != message.end())
    {
        if((*parser >= '0') && (*parser <= '9'))
        {
            int number = *parser - '0';
            lst.push_back(number);
        }

        parser++;
    }
}

void Server::displayList(std::list<int> &lst) const
{
    for(int &item: lst)
    {
        std::cout << item << " ";    
    }
    std::cout << std::endl;
}
