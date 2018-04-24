#ifndef IOSERVICE_H
#define IOSERVICE_H

#include <netinet/in.h>
#include <string>

class IOService
{
public:
    explicit IOService();
    ~IOService();
    unsigned short setProtocolType() const;
    void send_tcp(int socket, const char* message) const;
    void send_udp(int socket, const char* message, sockaddr_in* addr, socklen_t addrlen) const;
    bool receive_tcp(int socket, std::string& message) const;
    bool receive_udp(int socket, std::string& message, sockaddr_in* addr, socklen_t* addrlen) const;
    std::string getMessage() const;
    bool exit(std::string& message) const;
private:
    char* m_buffer;
};

#endif
