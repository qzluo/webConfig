/********************************************************************
 File: webSrvCmdParser.h
 Function: command parser to generate output from input
 Author: Luobihe
 Date: 2015-12-14
*********************************************************************/

#include "cmdParser.h"

#define WEBSRV_CMD_SET_SERVICE     0x01
#define WEBSRV_CMD_GET_SERVICE     0x02
#define WEBSRV_CMD_RESTART_SYS     0x03

#define WEBSRV_CMD_SERVICE_START   0x01
#define WEBSRV_CMD_SERVICE_STOP    0x02

#define WEBSRV_CMD_RSP_SUCCESS              0x01
#define WEBSRV_CMD_RSP_ERROR                0x02
#define WEBSRV_CMD_RSP_NOT_RECOGNIZED       0x03

//定义名令格式及命令响应结果
typedef struct {
    int cmd;
    char data[16];
} TagWebSrvCmd, TagWebSrvCmdBack;

int setService(char* p_data);

class CWebSrvCmdParser : public CCmdParser
{
public:
    virtual int parseCmd(const char* szIn, int iInLen, char* szOut, int* p_iRetlen);

    //split to get first cmd from message read
    virtual int getFirstCmd(const char* szIn, int iInLen, 
        int* p_iStartPos, int* p_iCmdLen);

private:
    void setResCmdNotRecognized(char* szOut, int* p_iRetlen);
};