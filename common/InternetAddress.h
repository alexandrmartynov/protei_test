#ifndef INTERNETADDRESS_H
#define INTERNETADDRESS_H

#include <netinet/in.h>

class InternetAddress
{
public:
    explicit InternetAddress() = default;
    InternetAddress(const InternetAddress& obj);
    void setup(int port);
    void binded(int socket) const;
    socklen_t getAddrSize() const;
    sockaddr_in& getAddress();
private:
    sockaddr_in m_addr;
    socklen_t m_addrlen;
};

#endif
