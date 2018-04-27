#ifndef CLIENT_TCPH
#define CLIENT_TCPH

#include "Socket_tcp.h"
#include <netinet/in.h>

class Client_tcp
{
public:
    Client_tcp();
    int exec(int port);
private:
    void writeInternetAddress();
    std::string getMessage() const;

    int m_port;
    Socket_tcp m_socket;
    sockaddr_in m_client_addr;
    sockaddr_in m_server_addr;
};

#endif
