/********************************************************************
 File: termioOp.cpp
 Function: Interface to operate.
 Author: Luobihe
 Date: 2016-1-6
*********************************************************************/

#ifndef _WIN32
#include <unistd.h>
#include <fcntl.h>
#endif

#include <stdio.h>
#include <string.h>
#include "termioOp.h"
#include "platcomm.h"
#include "serialCom.h"
#include "logsys.h"

static TermOpObj s_opObjs[] = {
    {TERMINAL_IO_TYPE_TCP, TcpOutput},
    {TERMINAL_IO_TYPE_UDP, UdpOutput},
    {TERMINAL_IO_TYPE_RS232, SerialComOutput},
};

static SerialFileMap s_serialFileTbl[] = {
    {"com2", "/dev/ttySAC1"},
    {"com3", "/dev/ttySAC2"},
    {"com4", "/dev/ttySAC3"},
};

TermOpObj* getTermOpObj(int type)
{
    for (int i = 0; i < sizeof(s_opObjs) / sizeof(s_opObjs[0]); i++) {
        if (type == s_opObjs[i].type)
            return &s_opObjs[i];
    }

    return NULL;
}

char* getSerialFileName(const char* desc)
{
    for (int i = 0; i < sizeof(s_serialFileTbl) / sizeof(s_serialFileTbl[0]); i++) {
        if (strcmp(desc, s_serialFileTbl[i].desc) == 0) {
            return s_serialFileTbl[i].devname;
        }
    }

    return NULL;
}

int TcpOutput(TerminalPara* p_paras, char* p_data, int datalen)
{
    if (p_paras->type != TERMINAL_IO_TYPE_TCP)
        return -1;

    return sendTcpData(p_paras->ip, p_paras->port, p_data, datalen);
}

int UdpOutput(TerminalPara* p_paras, char* p_data, int datalen)
{
    if (p_paras->type != TERMINAL_IO_TYPE_UDP)
        return -1;

    return sendUdpData(p_paras->ip, p_paras->port, p_data, datalen);
}

int SerialComOutput(TerminalPara* p_paras, char* p_data, int datalen)
{
    printf("SerialCom output\n");
    if (p_paras->type != TERMINAL_IO_TYPE_RS232)
        return -1;

    char* p_fileName = NULL;
    p_fileName = getSerialFileName(p_paras->desc);
    if (!p_fileName)
        return -1;

#ifndef _WIN32
    int fd = 0;
    fd = open(p_fileName, O_RDWR | O_NOCTTY);
    if (fd == -1) 
        return -1;

    //set speed
    if (set_speed(fd, p_paras->boud) == -1) {
        close(fd);
        return -1;
    }

    //set parity
    if (set_Parity(fd, p_paras->databits, p_paras->stopbits,
        p_paras->parity)) {
        close(fd);
        return -1;
    }

    if (write(fd, p_data, datalen) < 0) {
        close(fd);
        return -1;
    }

    close(fd);

#endif

    return 0;
}


