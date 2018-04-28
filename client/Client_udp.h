#ifndef CLIENT_UDPH
#define CLIENT_UDPH

#include <netinet/in.h>

class Client_udp
{
public:
    Client_udp();
    int exec(int port);
private:
    void setup(sockaddr_in& addr) const;

    int m_port;
};

#endif
