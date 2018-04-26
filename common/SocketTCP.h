#ifndef SOCKETTCP_H
#define SOCKETTCP_H

#include "Socket.h"
#include <netinet/in.h>

class SocketTCP: public Socket
{
public:
    SocketTCP();
    virtual ~SocketTCP();
    virtual int create() override;
    virtual void send(const std::string& message) const override;
    virtual std::string receive() override;
    void setSocket(int socket);
    void writeAddress(sockaddr_in);
private:
    int m_socket;
    sockaddr_in m_addr;
    sockaddr_in m_client_addr;
};

#endif
