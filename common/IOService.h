#ifndef IOSERVICE_H
#define IOSERVICE_H

#include <netinet/in.h>
#include <string>

class IOService
{
public:
    explicit IOService();
    ~IOService();
    void send_tcp(int socket, const char* message) const;
    void send_udp(int socket, const char* message, sockaddr_in* addr, socklen_t addrlen) const;
    std::string receive_tcp(int socket) const;
    std::string receive_udp(int socket, sockaddr_in* addr, socklen_t* addrlen) const;
    std::string getMessage() const;
    bool exit(const char* message) const;
private:
    char* m_buffer;
};

#endif
