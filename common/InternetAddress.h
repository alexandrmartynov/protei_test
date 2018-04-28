#ifndef INTERNETADDRESS_H
#define INTERNETADDRESS_H

#include <netinet/in.h>

class InternetAddress
{
public:
    InternetAddress() = default;
    void setup(int port);
    socklen_t getAddrSize() const;
    sockaddr_in& getAddress();
private:

    sockaddr_in m_addr;
};

#endif

