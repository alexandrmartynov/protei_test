#include "ClientUdp.h"
#include "SocketUdp.h"

int ClientUdp::exec(int port)
{
    SocketUdp socket;
    socket.setupAddress(port);
    socket.dialog();

    return 0;
}
