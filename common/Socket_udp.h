#ifndef SOCKETUDP_H
#define SOCKETUDP_H

#include "Socket.h"
#include "InternetAddress.h"

#include<string>

class Socket_udp: public Socket
{
public:
    explicit Socket_udp();
    virtual ~Socket_udp();
    virtual void create() override;
    virtual void closeSocket() override;

    int getSocket() const;
    void setSocket(int socket);

    void send(const std::string& message, InternetAddress& addr) const;
    std::string receive(InternetAddress& addr);
    void handle_message(InternetAddress& addr);
    std::string echo_message(InternetAddress& addr);
private:
    std::string getMessage() const;

    int m_socket;
};

#endif
