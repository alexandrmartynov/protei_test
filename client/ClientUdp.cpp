#include "ClientUdp.h"
#include "SocketUdp.h"
#include "IOService.h"

int ClientUdp::exec(int port)
{
    SocketUdp socket;
    IOService service;
    socket.setupAddress(port);
    service.dialog(&socket);

    return 0;
}
