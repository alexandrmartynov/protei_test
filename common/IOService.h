#ifndef IOSERVICE_H
#define IOSERVICE_H

#include <netinet/in.h>

class IOService
{
public:
    explicit IOService() = default;
    void send_tcp(int socket, char* message) const;
    void send_udp(int socket, char* message, sockaddr_in* addr, socklen_t addrlen) const;
    char* receive_tcp(int socket) const;
    char* receive_udp(int socket, sockaddr_in* addr, socklen_t* addrlen) const;
    char* numToStr(int number, size_t size) const;
private:

};

#endif
