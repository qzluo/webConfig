/********************************************************************
 File: termioOp.h
 Function: Interface to operate.
 Author: Luobihe
 Date: 2016-1-6
*********************************************************************/

#ifndef __TERMIOOP_H__
#define __TERMIOOP_H__

#include "termioManager.h"

typedef struct {
    int type;
    int (*opFun)(TerminalPara* p_paras, char* p_data, int datalen);
} TermOpObj;

typedef struct {
    char* desc;
    char* devname;
} SerialFileMap;

int SerialComOutput(TerminalPara* p_paras, char* p_data, int datalen);
int TcpOutput(TerminalPara* p_paras, char* p_data, int datalen);
int UdpOutput(TerminalPara* p_paras, char* p_data, int datalen);

TermOpObj* getTermOpObj(int type);
char* getSerialFileName(const char* desc);

#endif  //__TERMIOOP_H__