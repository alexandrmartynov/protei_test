#ifndef CLIENT_UDPH
#define CLIENT_UDPH

#include <InternetAddress.h>

class Client_udp
{
public:
    Client_udp() = default;
    int exec(int port);
private:

    InternetAddress server_addr;
};

#endif
