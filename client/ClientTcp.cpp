#include "ClientTcp.h"
#include "SocketTcp.h"
#include "IOService.h"

int ClientTcp::exec(int port)
{ 
    SocketTcp socket;
    IOService service;
    socket.setupAddress(port);
    socket.connected();
    service.dialog(&socket);

    return 0;
}
