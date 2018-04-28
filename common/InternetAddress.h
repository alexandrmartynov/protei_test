#ifndef INTERNETADDRESS_H
#define INTERNETADDRESS_H

#include <netinet/in.h>

class InternetAddress
{
public:
    explicit InternetAddress() = default;
    void setup(int port);
    void binded(int socket);
    socklen_t getAddrSize() const;
    sockaddr_in& getAddress();
private:

    sockaddr_in m_addr;
};

#endif

