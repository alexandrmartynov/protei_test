#ifndef CLIENT_UDPH
#define CLIENT_UDPH

#include "Socket_udp.h"
#include <netinet/in.h>

class Client_udp
{
public:
    Client_udp();
    int exec(int port);
private:
    void writeInternetAddress();
    std::string getMessage() const;

    int m_port;
    Socket_udp m_socket;
    sockaddr_in m_client_addr;
    sockaddr_in m_server_addr;
};

#endif
