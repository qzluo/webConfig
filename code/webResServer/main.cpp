#include "platcomm.h"
#include "tcplistener.h"

int main(int argc, char* argv[])
{
    initSocket();

    startTcpListen();

    deinitSocket();

    return 0;
}