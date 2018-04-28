#include "Client_tcp.h"
#include "Socket_tcp.h"

int Client_tcp::exec(int port)
{ 
    m_server_addr.setup(port);
    Socket_tcp socket;
    socket.create();
    socket.connected(m_server_addr);
    socket.handle_message();
    socket.closeSocket();

    return 0;

}
