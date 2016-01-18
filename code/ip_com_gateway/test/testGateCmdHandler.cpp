#include <stdio.h>
#include <string.h>
#include "testGateCmdHandler.h"

void CTestGateCmdHandler::printTerminalPara(TerminalPara* p_data)
{
    if (p_data->type == TERMINAL_IO_TYPE_TCP || 
        p_data->type == TERMINAL_IO_TYPE_UDP) {
        printf("desc = %s, ip = %s, port = %d\n", p_data->desc, p_data->ip, p_data->port);
    }
    else {
        printf("desc = %s, bord = %d, databits = %d, stopbits = %d, parity = %d\n",
            p_data->desc, p_data->boud, p_data->databits, 
            p_data->stopbits, p_data->parity);
    }
}

void CTestGateCmdHandler::printList(list <char*>* p_list)
{
    list <char*>::iterator pos = p_list->begin();
    list <char*>::iterator last = p_list->end();

    int listLen = p_list->size();
    printf("list length = %d\n", listLen);

    for (;pos != last; pos++) {
        printf("%s\n", *pos);
    }    
}

void CTestGateCmdHandler::testCmdHandleSingleExp(void)
{
#if 0
    printf("======================test CmdHandleSingleExp=====================\n");
    char in_data[256] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
    int ret = 0;
    ret = m_inst.cmdHandleSingleExp("tcp 192.168.1.11:com3:0x01 0x02 0x03 0x04 0x05 0x06:"
        "0xa7 0x08 0x09 0x0a 0x0b 0x0c",
        "tcp 192.168.1.11",
        in_data,
        6);

    if (ret)
        printf("trans failed\n");
    else
        printf("trans success\n");
#endif
}

void CTestGateCmdHandler::testTansDataFromRule(void)
{
#if 0
    printf("======================test TansDataFromRule=====================\n");
    char tarInfo[256] = {0};
    char out_data[256] = {0};
    char in_data[256] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
    int ret = 0;
    CGateCmdHandler* p_inst = CGateCmdHandler::getInstance();
    ret = p_inst->tansDataFromRule("tcp 192.168.1.11:com3:0x01 0x02 0x03 0x04 0x05 0x06:"
        "0x07 0x08 0x09 0x0a 0x0b 0x0c",
        "tcp 192.168.1.11",
        in_data,
        6,
        tarInfo, out_data);

    if (ret)
        printf("trans failed\n");
    else
        printf("trans success, tarInfo = %s, out_data = %s\n",
        tarInfo, out_data);
#endif
}

void CTestGateCmdHandler::testCompareSrcInfo(void)
{
#if 0
    printf("======================test CompareSrcInfo=====================\n");
    char info_from_src[60] = {0};
    char info_from_exp[60] = {0};

    strcpy(info_from_src, "  tcp   192.168.1.112");
    strcpy(info_from_exp, " tcp 192.168.1.112");

    if (m_inst.compareSrcInfo(info_from_src, info_from_exp))
        printf("1. %s.\n2. %s\nnot match\n", 
        info_from_src, info_from_exp);
    else
        printf("1. %s.\n2. %s\nmatch\n", 
        info_from_src, info_from_exp);

    strcpy(info_from_src, "  tcp   192.168.1.112");
    strcpy(info_from_exp, " tcp 192.168.1.113");

    if (m_inst.compareSrcInfo(info_from_src, info_from_exp))
        printf("1. %s.\n2. %s\nnot match\n", 
        info_from_src, info_from_exp);
    else
        printf("1. %s.\n2. %s\nmatch\n", 
        info_from_src, info_from_exp);

    strcpy(info_from_src, "  tcp   192.168.1.112");
    strcpy(info_from_exp, " com 192.168.1.112");

    if (m_inst.compareSrcInfo(info_from_src, info_from_exp))
        printf("1. %s.\n2. %s\nnot match\n", 
        info_from_src, info_from_exp);
    else
        printf("1. %s.\n2. %s\nmatch\n", 
        info_from_src, info_from_exp);

    strcpy(info_from_src, "  tcp   192.168.1.112");
    strcpy(info_from_exp, " tcp 255.255.255.255");

    if (m_inst.compareSrcInfo(info_from_src, info_from_exp))
        printf("1. %s.\n2. %s\nnot match\n", 
        info_from_src, info_from_exp);
    else
        printf("1. %s.\n2. %s\nmatch\n", 
        info_from_src, info_from_exp);
#endif
}

void CTestGateCmdHandler::testGenDataFromText(void)
{
#if 0
    printf("======================test GenDataFromText=====================\n");

    char out_data[256] = {0};
    int datalen = 0;
    int ret = 0;
    CGateCmdHandler* p_inst = CGateCmdHandler::getInstance();
    ret = p_inst->genDataFromText("0x01294    0x02    0x03   0x04 0x05 0x06",
        out_data, &datalen);

    if (ret)
        printf("trans failed\n");
    else {
        printf("datalen = %d\n", datalen);
        for (int i = 0; i < datalen; i++) {
            printf("0x%02x ", out_data[i]);
        }
    }
#endif
}

void CTestGateCmdHandler::testInit(void)
{
    printf("======================test Init=====================\n");
    list <char*>* p_list = NULL;
    CGateCmdHandler* p_inst = CGateCmdHandler::getInstance();
    p_list = p_inst->getList();

    printf("before init\n");
    printList(p_list);

    p_inst->init();

    printf("after init\n");
    printList(p_list);
}

void CTestGateCmdHandler::testCmdHandle(void)
{
    printf("======================test CmdHandle=====================\n");
    char in_data[256] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
    int ret = 0;
    CGateCmdHandler* p_inst = CGateCmdHandler::getInstance();
    p_inst->init();
    p_inst->cmdHandle("tcp 192.168.1.11",
        in_data,
        6);

    printf("command handle finish\n");
}

void CTestGateCmdHandler::testGetTermParas(void)
{
#if 0
    printf("======================test GetTermParas=====================\n");
    int ret = 0;
    TerminalPara paras = {0};
    TerminalPara* p_data = &paras;
    char szInfo[256] = {0};

    printf("test tcp\n");
    strcpy(szInfo, "tcp 192.168.1.123 1234");

    ret = m_inst.getTermParas(szInfo, &paras);
    if (ret)
        printf("Term not found\n");
    else {
        printTerminalPara(&paras);
    }

    printf("test udp\n");
    strcpy(szInfo, "udp 192.168.1.123 1234");

    ret = m_inst.getTermParas(szInfo, &paras);
    if (ret)
        printf("Term not found\n");
    else {
        printTerminalPara(&paras);
    }

    printf("test com2\n");
    strcpy(szInfo, "com2");

    ret = m_inst.getTermParas(szInfo, &paras);
    if (ret)
        printf("Term not found\n");
    else {
        printTerminalPara(&paras);
    }

    printf("test xxx\n");
    strcpy(szInfo, "com2");

    ret = m_inst.getTermParas(szInfo, &paras);
    if (ret)
        printf("Term not found\n");
    else {
        printTerminalPara(&paras);
    }
#endif
}

void CTestGateCmdHandler::testOutput(void)
{
#if 0
    printf("======================test Output=====================\n");
    int ret = 0;
    char szInfo[256] = {0};

    printf("test tcp\n");
    strcpy(szInfo, "tcp 192.168.1.123 1234");

    CGateCmdHandler* p_inst = CGateCmdHandler::getInstance();
    ret = p_inst->putoutData(szInfo, NULL, 0);
    if (ret)
        printf("output failed\n");
    else 
        printf("output success\n");

    printf("test udp\n");
    strcpy(szInfo, "udp 192.168.1.123 1234");

    ret = p_inst->putoutData(szInfo, NULL, 0);
    if (ret)
        printf("output failed\n");
    else 
        printf("output success\n");

    printf("test com\n");
    strcpy(szInfo, "com2");

    ret = p_inst->putoutData(szInfo, NULL, 0);
    if (ret)
        printf("output failed\n");
    else 
        printf("output success\n");
#endif
}

void CTestGateCmdHandler::test(void)
{
 //   testTansDataFromRule();
    testGenDataFromText();
    //testCmdHandleSingleExp();
    //testInit();
    //testCmdHandle();
   // testGetTermParas();
    //testOutput();
}
