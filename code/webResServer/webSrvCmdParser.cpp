/********************************************************************
 File: webSrvCmdParser.cpp
 Function: command parser to generate output from input
 Author: Luobihe
 Date: 2015-12-14
*********************************************************************/

#include <stdlib.h>
#include <string.h>
#include "webSrvCmdParser.h"

#ifndef _WIN32
#include <unistd.h>
#endif

/*-------------------------------------------------------------------
 Function: setService(p_data);
           启停主机服务进程
 Parameters:
 Return: 0 -- success
         -1 -- failed
-------------------------------------------------------------------*/
int setService(char* p_data)
{
#ifndef _WIN32

    char file_path[256] = {0};
    pid_t pid = 0;
    const char* opPara = NULL;

    if (!p_data)
        return -1;

    opPara = (p_data[0] == WEBSRV_CMD_SERVICE_START) ?
        "start" : "stop";

    pid = vfork();
    if (pid < 0)
        return -1;
    else if (pid == 0) {
        strcpy(file_path, "/home/sky/ehomeserver/bin/ehservice");
        execl(file_path, file_path, opPara, NULL);
        exit(-1);
    }

#endif
    return 0;
}

int CWebSrvCmdParser::parseCmd(const char* szIn, int iInLen, char* szOut, int* p_iRetlen)
{
    TagWebSrvCmd cmdParas = {0};
    TagWebSrvCmdBack cmdBackParas = {0};
    int ret = 0;

    if (iInLen < sizeof(TagWebSrvCmd)) {
        setResCmdNotRecognized(szOut, p_iRetlen);
        return -1;
    }

    memcpy(&cmdParas, szIn, sizeof(TagWebSrvCmd));
    switch (cmdParas.cmd) {
        case WEBSRV_CMD_SET_SERVICE:
            ret = setService(cmdParas.data);

            cmdBackParas.cmd = (ret == 0) ?
                WEBSRV_CMD_RSP_SUCCESS :
                WEBSRV_CMD_RSP_ERROR;

            memcpy(szOut, &cmdBackParas, sizeof(TagWebSrvCmdBack));
            *p_iRetlen = sizeof(TagWebSrvCmdBack);

            break;

        case WEBSRV_CMD_GET_SERVICE:
            setResCmdNotRecognized(szOut, p_iRetlen);
            ret = -1;
            break;

        default:
            setResCmdNotRecognized(szOut, p_iRetlen);
            ret = -1;
            break;
    }

    return ret;
}

//split to get first cmd from message read
int CWebSrvCmdParser::getFirstCmd(const char* szIn, int iInLen, 
                                  int* p_iStartPos, int* p_iCmdLen)
{
    return 0;
}

void CWebSrvCmdParser::setResCmdNotRecognized(char* szOut, int* p_iRetlen)
{
    TagWebSrvCmdBack cmdBackParas = {0};
    cmdBackParas.cmd = WEBSRV_CMD_RSP_NOT_RECOGNIZED;
    memcpy(szOut, &cmdBackParas, sizeof(TagWebSrvCmdBack));
    *p_iRetlen = sizeof(TagWebSrvCmdBack);
}
