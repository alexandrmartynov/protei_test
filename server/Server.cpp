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
    ev.events = EPOLLIN | EPOLLOUT;
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

    m_socket_udp.create();
    int sock_udp = m_socket_udp.getSocket();
    setnonblocking(sock_udp);

    m_socket_udp.binded(m_server_addr);


    m_socket_tcp.create();
    m_socket_tcp.binded(m_server_addr);
    int listen_sock = m_socket_tcp.getSocket();

    setnonblocking(listen_sock);

    m_socket_tcp.listening();

    socklen_t client_addrlen = sizeof(m_client_addr);

    int nfds = 0;
    int epollfd = 0;
    epoll_event ev;
    epoll_event events[2];

    epollfd = epoll_create(2);

    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = sock_udp;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock_udp, &ev) == -1)
    {
       std::cout << "failed epoll_ctl" << std::endl;
       return EXIT_FAILURE;
    }

    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listen_sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1)
    {
       std::cout << "failed epoll_ctl" << std::endl;
       return EXIT_FAILURE;
    }
    
    bool exit = false;
    while(!exit)
    {
        std::cout << "Waiting connection...\n";
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds <= -1)
        {
            std::cout << "epoll_wait failed" << std::endl;
            return EXIT_FAILURE;
        }

        memset(m_buffer, 0, BUFFER_SIZE);
        std::string message = {};
        int client_socket = 0;
        socklen_t client_addrlen = sizeof(m_client_addr);
        for (int n = 0; n < nfds; ++n)
        {
            if(events[n].data.fd == listen_sock)
            {
                std::cout << "TCP\n";
                client_socket = m_socket_tcp.accepted(&m_client_addr, &client_addrlen);
                if(client_socket < 0)
                {
                    printf("accept failed %d\n", errno);
                    return EXIT_FAILURE;
                }

                setnonblocking(client_socket);
        
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = client_socket;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, client_socket, &ev) == -1)
                {
                    std::cout << "failed epoll_ctl" << std::endl;
                    return EXIT_FAILURE;
                }
            }
            else if(events[n].data.fd == sock_udp & EPOLLIN)
            {
                std::cout << "UDP\n";
                m_socket_udp.setSocket(events[n].data.fd);
                bool close = false;
                while(!close)
                {
                    close = m_socket_udp.handle_message(m_client_addr, client_addrlen);
                }
                m_socket_udp.disconnect();

                if (epoll_ctl(epollfd, EPOLL_CTL_DEL, events[n].data.fd, &ev) == -1)
                {
                    std::cout << "failed epoll_ctl" << std::endl;
                    return EXIT_FAILURE;
                }
            }
            else
            {
                std::cout << "TCP\n";
                m_socket_tcp.setSocket(events[n].data.fd);
                bool close = false;
                while(!close)
                {
                    close = m_socket_tcp.handle_message();
                }
                m_socket_tcp.disconnect();                   
                if (epoll_ctl(epollfd, EPOLL_CTL_DEL, events[n].data.fd, &ev) == -1)
                {
                    std::cout << "failed epoll_ctl" << std::endl;
                    return EXIT_FAILURE;
                }
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
        if(isdigit(*parser))
        {
            int number = *parser + '0';
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
