#include "Client_udp.h"
#include "Socket_udp.h"

int Client_udp::exec(int port)
{
    m_server_addr.setup(port);
    Socket_udp socket;
    socket.create();
    socket.handle_message(m_server_addr);
    socket.closeSocket();

    return 0;
}
