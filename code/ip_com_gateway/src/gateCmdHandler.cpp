/********************************************************************
 File: gateCmdHandler.cpp
 Function: Transfer the source data to the target data. 
           Output the target data.
 Author: Luobihe
 Date: 2016-1-4
*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gateCmdHandler.h"
#include "platcomm.h"
#include "termioOp.h"

static char* szExp[] = {
    "tcp 192.168.1.11:com3:0x01 0x02 0x03 0x04 0x05 0x06:0xa7 0x08 0x09 0x0a 0x0b 0x0c",
    "tcp 192.168.1.11:com4:0x11 0x12 0x13 0x14 0x15 0x16:0x17 0x18 0x19 0x1a 0x1b 0x1c",
    "tcp 192.168.1.11:com2:0x21 0x22 0x23 0x24 0x25 0x26:0x27 0x28 0x29 0x2a 0x2b 0x2c",
    "tcp 192.168.1.11:com2:0x31 0x32 0x33 0x34 0x35 0x36:0x37 0x38 0x39 0x3a 0x3b 0x3c",
    "tcp 192.168.1.11:com3:0x41 0x42 0x43 0x44 0x45 0x46:0x47 0x48 0x49 0x4a 0x4b 0x4c",
    "tcp 192.168.1.11:com4:0x01 0x02 0x03 0x04 0x05 0x06:0x57 0x08 0x09 0x0a 0x0b 0x0c",
};

int initObj(void)
{
    if (CGateCmdHandler::getInstance()->init())
        return -1;
    
    return CTermManager::getInstance()->init();
}

CGateCmdHandler* CGateCmdHandler::m_pInst = NULL;

CGateCmdHandler* CGateCmdHandler::getInstance(void)
{
    if (!m_pInst)
        m_pInst = new CGateCmdHandler();

    return m_pInst;
}

CGateCmdHandler::CGateCmdHandler(void)
{
}

CGateCmdHandler::~CGateCmdHandler(void)
{
    clearList();
}

void CGateCmdHandler::clearList(void)
{
    char* p_data = NULL;
    list <char*> ::iterator pos = m_listTransExp.begin();
    list <char*> ::iterator last = m_listTransExp.end();

    while (pos != last) {
        p_data = *pos;

        pos = m_listTransExp.erase(pos);
        delete p_data;
        p_data = NULL;
    }
}

//读数据转化文件，生成转换列表
int CGateCmdHandler::init(void)
{
    char fileName[256] = {0};
    char fileDir[256] = {0};
    char processName[256] = {0};
    FILE* p_file = NULL;
    char szExp[1024] = {0};    

    clearList();

    if (getExePath(fileDir, processName, 256))
        return -1;   

    sprintf(fileName, "%s/%s", fileDir, FILE_DATA_TRANS);

    p_file = fopen(fileName, "r");
    if (!p_file)
        return -1;

    while (fgets(szExp, 1024, p_file)) {
        if (addExpression(szExp)) {
            fclose(p_file);
            return -1;
        }
    }

    fclose(p_file);
    
    return 0;
}

//命令处理
void CGateCmdHandler::cmdHandle(const char* srcInfo, 
                                char* in_data, 
                                int in_datalen)
{    
    char* p_express = NULL;
    list <char*> :: iterator pos = m_listTransExp.begin();
    list <char*> :: iterator last = m_listTransExp.end();
    for (; pos != last; pos++) {
        p_express = *pos;
        cmdHandleSingleExp(p_express, srcInfo, in_data, in_datalen);
    }
}

/*-------------------------------------------------------------------
 Function: cmdHandleSingleExp(...)
 Purpose: 根据表达式，由输入源信息及输入数据执行命令
 Parameters: expression -- [IN], expression with the following fmt:
                tcp xxx.xxx.xxx.xxx:tarInfo:in_data(text):out_data(text)
             srcInfo -- [IN], infomation about data source
             in_data -- [IN], data to transfered
             in_datalen -- [IN], data length to transfered
 return: 0 --  success
         -1 -- failed
-------------------------------------------------------------------*/
int CGateCmdHandler::cmdHandleSingleExp(const char* expression, 
                                        const char* srcInfo, 
                                        char* in_data, 
                                        int in_datalen)
{
    char out_dataText[MAX_DATA_LENGTH] = {0};
    char out_data[MAX_DATA_LENGTH] = {0};
    char tar_info[MAX_INFO_LENGTH] = {0};
    int out_datalen = 0;
    if (tansDataFromRule(expression, srcInfo, in_data, in_datalen, 
        tar_info, out_dataText))
        return -1;
    
    //透传情况处理
    if (strncmp(out_dataText, "*", 1) == 0) {
        memcpy(out_data, in_data, in_datalen);
        out_datalen = in_datalen;
    }
    else {
        if (genDataFromText(out_dataText, out_data, &out_datalen))
            return -1;
    }

    //命令执行
    if (putoutData(tar_info, out_data, out_datalen))
        return -1;

    return 0;
}

/*-------------------------------------------------------------------
 Function: tansDataFromRule(...)
 Purpose: 根据表达式，由输入源信息及输入数据产生输出端口信息及输出数据
 Parameters: expression -- [IN], expression with the following fmt:
                tcp xxx.xxx.xxx.xxx:tarInfo:in_data(text):out_data(text)
             srcInfo -- [IN], infomation about data source
             in_data -- [IN], data to transfered
             in_datalen -- [IN], data length to transfered
             tarInfo -- [OUT], information about target data
             out_data -- [OUT], transfered data to output
 return: 0 --  success
         -1 -- failed
-------------------------------------------------------------------*/
int CGateCmdHandler::tansDataFromRule(const char* expression, 
                                      const char* srcInfo, 
                                      char* in_data,
                                      int in_datalen, 
                                      char* tarInfo, 
                                      char* out_data)
{
    char szSrcInfoFromExp[64] = {0};
    char szInDataFromExp[256] = {0};
    char szTarInfoFromExp[64] = {0};
    char szOutDataFromExp[256] = {0};

    if (!expression)
        return -1;

    //check : count

    sscanf(expression, "%[^:]%*c%[^:]%*c%[^:]%*c%[^\r\n]",
        szSrcInfoFromExp, szTarInfoFromExp,
        szInDataFromExp, szOutDataFromExp);

    //验证输入源信息是否匹配
    if (compareSrcInfo(srcInfo, szSrcInfoFromExp))
        return -1;

    //透传数据情况处理
    if (strncmp(szInDataFromExp, "*", 1) == 0) {
        strcpy(tarInfo, szTarInfoFromExp);
        strcpy(out_data, "*");
        return 0;
    }

    //验证输入数据是否匹配
    if (compareInData(in_data, in_datalen, szInDataFromExp))
        return -1;

    strcpy(tarInfo, szTarInfoFromExp);
    strcpy(out_data, szOutDataFromExp);

    return 0;
}

/*-------------------------------------------------------------------
 Function: putoutData(...)
 Purpose: 输出数据
 Parameters: tarInfo -- [IN], information about target data           
             out_data -- [IN], data to output
             out_datalen -- [IN], data length to output
 return: 0 --  success
         -1 -- failed
-------------------------------------------------------------------*/
int CGateCmdHandler::putoutData(const char* tarInfo, 
                                char* out_data, 
                                int out_datalen)
{
    TerminalPara paras = {0};
    TermOpObj* p_termOpObj = NULL;

    //取得接口参数
    if (getTermParas(tarInfo, &paras))
        return -1;

    //取得操作接口
    p_termOpObj = getTermOpObj(paras.type);
    if (!p_termOpObj)
        return -1;

    //输出数据
    return p_termOpObj->opFun(&paras, out_data, out_datalen);
}

/*-------------------------------------------------------------------
 Function: compareSrcInfo(...)
 Purpose: 验证输入端口是否是需要转换的数据
 Parameters: info_from_src -- [IN], infomation from caller              
             info_from_exp -- [IN], infomation from expression
 return: 0 --  success
         -1 -- failed
-------------------------------------------------------------------*/
int CGateCmdHandler::compareSrcInfo(const char* info_from_src, 
                                    const char* info_from_exp)
{
    char info_header_from_src[16] = {0};
    char info_header_from_exp[16] = {0};
    char info_content_from_src[64] = {0};
    char info_content_from_exp[64] = {0};

    if (info_from_src[0] == ' ')
        sscanf(info_from_src, "%*[ ]%[^ ]%*[ ]%[^\n\0]",
        info_header_from_src, info_content_from_src);
    else
        sscanf(info_from_src, "%[^ ]%*[ ]%[^\n\0]",
        info_header_from_src, info_content_from_src);

    if (info_from_exp[0] == ' ')
        sscanf(info_from_exp, "%*[ ]%[^ ]%*[ ]%[^\n\0]",
        info_header_from_exp, info_content_from_exp);
    else
        sscanf(info_from_exp, "%[^ ]%*[ ]%[^\n\0]",
        info_header_from_exp, info_content_from_exp);

    if (strcmp(info_header_from_src, info_header_from_exp))
        return -1;

    //当表达式是 255.255.255.255 时，不考虑输入源
    if (strcmp(info_content_from_exp, "255.255.255.255") == 0)
        return 0;

    if (strcmp(info_content_from_src, info_content_from_exp))
        return -1;

    return 0;
}

/*-------------------------------------------------------------------
 Function: compareInData(...)
 Purpose: 比较输入的数据跟表达式的数据，看是否匹配
 Parameters: in_data -- [IN], data to transfered
             in_datalen -- [IN], data length to transfered
             indata_text -- [IN], data in text format
 return: 0 -- match
         -1 -- not match
-------------------------------------------------------------------*/
int CGateCmdHandler::compareInData(char* in_data, 
                                   int in_datalen, 
                                   const char* indata_text)
{
    char in_data_from_text[256] = {0};
    int in_datalen_from_text = 0;

    if (genDataFromText(indata_text, in_data_from_text, &in_datalen_from_text))
        return -1;

    if (in_datalen_from_text != in_datalen)
        return -1;

    //same data len
    for (int i = 0; i < in_datalen; i++) {
        if (in_data[i] != in_data_from_text[i])
            return -1;
    }

    return 0;
}

/*-------------------------------------------------------------------
 Function: genDataFromText(...)
 Purpose: 从16进制文本生成数据
 Parameters: data_text -- [IN], text format data
             out_data -- [OUT], data result
             datalen -- [OUT], data length
 return: 0 -- success
         -1 -- failed
-------------------------------------------------------------------*/
int CGateCmdHandler::genDataFromText(const char* data_text, 
                                     char* out_data, 
                                     int* datalen)
{
    char* p_start = NULL;
    char* p_end = NULL;
    int i = 0;
    p_start = (char*)data_text;

    while (*p_start) {
        while (*p_start == ' ')
            p_start++;

        if (!*p_start)
            break;

        out_data[i++] = 0xff & strtoul(p_start, &p_end, 16);
        p_start = p_end;
    }

    *datalen = i;

    return 0;
}

int CGateCmdHandler::addExpression(const char* exp)
{
    char* p_data = NULL;
    p_data = new char[strlen(exp) + 1];
    if (!p_data)
        return -1;

    sscanf(exp, "%*[^:]%*c%[^\r\n]", p_data);
    m_listTransExp.push_back(p_data);

    return 0;
}

/*-------------------------------------------------------------------
 Function: getTermParas(...)
 Purpose: 由端口字符标识，取得接口参数
 Parameters: tarInfo -- [IN], 端口字符标识
             p_paras -- [OUT], 参数结果
 return: 0 -- success
         -1 -- failed
-------------------------------------------------------------------*/
int CGateCmdHandler::getTermParas(const char* tarInfo, 
                                  TerminalPara* p_paras)
{
    if (strncmp(tarInfo, TERMINAL_IO_DESC_TCP, 
        strlen(TERMINAL_IO_DESC_TCP)) == 0) { //tcp type
        p_paras->type = TERMINAL_IO_TYPE_TCP;
        strcpy(p_paras->desc, TERMINAL_IO_DESC_TCP);
        sscanf(tarInfo, "%*s%s%d", p_paras->ip, &(p_paras->port));

        return 0;
    }
    else if (strncmp(tarInfo, TERMINAL_IO_DESC_UDP, 
        strlen(TERMINAL_IO_DESC_UDP)) == 0) { //udp type
        p_paras->type = TERMINAL_IO_TYPE_UDP;
        strcpy(p_paras->desc, TERMINAL_IO_DESC_UDP);
        sscanf(tarInfo, "%*s%s%d", p_paras->ip, &(p_paras->port));

        return 0;
    }
    else { //com type
        CTermManager* p_termManager = CTermManager::getInstance();
        if (!p_termManager)
            return -1;

        return p_termManager->getTermParas(tarInfo, p_paras);
    }

    return -1;
}
