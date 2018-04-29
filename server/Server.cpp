#include "Server.h"
#include "Socket_tcp.h"
#include "Socket_udp.h"
#include "Epoll.h"

#include <iostream>
#include <fcntl.h>
#include <sys/epoll.h>

#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <typeinfo>
#include <algorithm>

#define LOCALHOST "127.0.0.1"
#define PORT 8080

Server::~Server()
{}

int Server::exec()
{
    Epoll epoll(MAX_EVENTS);

    m_server_addr.setup(PORT);
    Socket_udp m_socket_udp;
    m_socket_udp.create();
    int sock_udp = m_socket_udp.getSocket();
    setnonblocking(sock_udp);
    m_server_addr.binded(sock_udp);

    Socket_tcp m_socket_tcp;
    m_socket_tcp.create();
    int listen_sock = m_socket_tcp.getSocket();
    setnonblocking(listen_sock);
    m_server_addr.binded(listen_sock);
    m_socket_tcp.listening();

    epoll.createEvents();
    epoll.createEpollfd();

    epoll.addEvent(sock_udp);
    epoll.addEvent(listen_sock);
    
    int nfds = 0;
    while(true)
    {
        m_socket_tcp.setSocket(listen_sock);
        
        nfds = epoll.wait();

        int sock_tcp = 0;
        std::string message = {};
        for (int n = 0; n < nfds; ++n)
        {
            int fd = epoll.getfd(n);
            if(fd == listen_sock)
            {
                sock_tcp = m_socket_tcp.accepted(m_client_addr);
                if(sock_tcp < 0)
                {
                    std::cout << "accept failed with error " << strerror(errno) << std::endl;
                    printf("accept failed %d\n", errno);
                    return EXIT_FAILURE;
                }

                setnonblocking(sock_tcp);
                epoll.addEvent(sock_tcp);
            }
            else if(fd == sock_udp)
            {
                std::cout << "UDP connect" << std::endl;
                m_socket_udp.setSocket(fd);
                message.clear();
                message = m_socket_udp.echo_message(m_server_addr);
                if(message.compare("-exit") != 0)
                {
                    result(message);
                }
            }
            else
            {
                std::cout << "TCP connect" << std::endl;
                m_socket_tcp.setSocket(fd);
                message.clear();
                message = m_socket_tcp.echo_message();
                if(message.compare("-exit") != 0)
                {
                    result(message);
                }
            }
        }
    }

    int epollfd = epoll.getEpollfd();
    close(epollfd);

    return 0;

}

int Server::setnonblocking(int socket)
{
    int result = -1;
    int flag = fcntl(socket, F_GETFL);
    
    if(flag >= 0)
    {

        flag = (flag | O_NONBLOCK);
        if(fcntl(socket, F_SETFL, flag) >= 0)
        {
            result = 0;
            std::cout << "Flag set successful" << std::endl;
        }
    }

    return result;

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
