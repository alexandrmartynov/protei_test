#include "Server.h"

int main(int /*argc*/, char** /*argv*/)
{
    Server server;
    int result = server.exec();
    return result;    
}
