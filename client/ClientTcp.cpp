#include "ClientTcp.h"
#include "SocketTcp.h"

int ClientTcp::exec(int port)
{ 
    SocketTcp socket;
    socket.setupAddress(port);
    socket.connected();
    socket.dialog();

    return 0;
}
