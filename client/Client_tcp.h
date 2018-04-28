#ifndef CLIENT_TCPH
#define CLIENT_TCPH

#include <netinet/in.h>

class Client_tcp
{
public:
    Client_tcp();
    int exec(int port);
private:
    void setup(sockaddr_in& addr) const;

    int m_port;
};

#endif
