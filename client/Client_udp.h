#ifndef CLIENT_UDPH
#define CLIENT_UDPH

#include <InternetAddress.h>

class Client_udp
{
public:
    explicit Client_udp() = default;
    int exec(int port);
private:

//    InternetAddress m_server_addr;
};

#endif
