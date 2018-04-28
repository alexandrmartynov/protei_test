#ifndef SOCKETUDP_H
#define SOCKETUDP_H

#include "Socket.h"
#include "InternetAddress.h"

class Socket_udp: public Socket
{
public:
    Socket_udp();
    virtual ~Socket_udp();
    virtual void create() override;
    void binded(InternetAddress& m_addr);
    int getSocket() const;
    void setSocket(int socket);
    void closeSocket();

    void send(const std::string& message, InternetAddress& addr) const;
    std::string receive(InternetAddress& addr);
    void handle_message(InternetAddress& addr);
    std::string echo_message(InternetAddress& addr);
private:
    std::string getMessage() const;

    int m_socket;
};

#endif
