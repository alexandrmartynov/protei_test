#include "ClientTcp.h"
#include "SocketTcp.h"

int ClientTcp::exec(int port)
{ 
    SocketTcp socket;
    socket.setup(port);
    socket.connected();
    socket.handle_message();
    socket.closeSocket();

    return 0;

}
