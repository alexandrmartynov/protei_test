#ifndef CLIENT_TCPH
#define CLIENT_TCPH

#include "InternetAddress.h"

class Client_tcp
{
public:
    explicit Client_tcp() = default;
    int exec(int port);
private:

    InternetAddress m_server_addr;
};

#endif
