#ifndef SERVER_H
#define SERVER_H

#include "Socket_tcp.h"
#include "Socket_udp.h"

#include <netinet/in.h>
#include <list>

class Server
{
public:
    Server();
    ~Server();
    int exec();
private:
    void addEvent(int epollfd, int fd) const;
    int setnonblocking(int socket);

    void result(std::string& message) const;
    void displayList(std::list<int> &lst) const;
    void parse(std::string& message, std::list<int> &lst) const;

    static const unsigned int MAX_EVENTS = 10;
    static const unsigned int BUFFER_SIZE = 1024;
    
    Socket_tcp m_socket_tcp;
    Socket_udp m_socket_udp;
    sockaddr_in m_client_addr;
    sockaddr_in m_server_addr;
    char* m_buffer;
};

#endif
