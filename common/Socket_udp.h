#ifndef SOCKETUDP_H
#define SOCKETUDP_H

#include "Socket.h"
#include <netinet/in.h>

class Socket_udp: public Socket
{
public:
    Socket_udp();
    virtual ~Socket_udp();
    virtual void create() override;
    void binded(sockaddr_in& m_addr);

    void send(const std::string& message, sockaddr_in& addr) const;
    std::string receive(sockaddr_in& addr);
    int getSocket() const;
    void setSocket(int socket);
    void handle_message(sockaddr_in& addr);
    std::string echo_message(sockaddr_in& addr);
private:
    std::string getMessage() const;

    int m_socket;
};

#endif
