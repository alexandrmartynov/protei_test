#include "ClientUdp.h"
#include "SocketUdp.h"

int ClientUdp::exec(int port)
{
    SocketUdp socket;
    socket.setup(port);
    socket.createSocket();
    socket.handle_message();
    socket.closeSocket();

    return 0;
}
