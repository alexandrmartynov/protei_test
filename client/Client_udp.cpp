#include "Client_udp.h"
#include "Socket_udp.h"

int Client_udp::exec(int port)
{
    server_addr.setup(port);
    Socket_udp socket;
    socket.create();
    socket.handle_message(server_addr);
    socket.closeSocket();
    return 0;

}

/*void Client_udp::setup(sockaddr_in& addr) const
{
    socklen_t addrlen = sizeof(addr);
    std::memset(reinterpret_cast<char*>(&addr), 0 , addrlen);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(m_port);
    int convert = inet_aton(LOCALHOST, reinterpret_cast<in_addr*>(&addr.sin_addr.s_addr));
    if(convert == 0)
    {
        std::cout << "inet_aton() failed with " << strerror(errno) << std::endl;
        return;
    }  
}*/
