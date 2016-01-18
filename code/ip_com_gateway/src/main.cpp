#include <stdio.h>
#include "platcomm.h"
#include "tcplistener.h"
#include "termioManager.h"
#include "gateCmdHandler.h"
#include "testTcpListener.h"
#include "testTermioManager.h"
#include "testGateCmdHandler.h"
#include "logsys.h"

int main(int argc, char* argv[])
{
    if (initSocket())
        return -1;

    if (initObj())
        return -1;

    loginit("log.dat", LOG_DEBUG);

    while (1) {
        platSleep(10);
    }

    deinitSocket();

    return 0;
}
