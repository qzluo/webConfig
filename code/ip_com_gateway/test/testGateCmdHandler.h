#ifndef __TEST_GATECMDHANDLER_H__
#define __TEST_GATECMDHANDLER_H__

#include "gateCmdHandler.h"

class CTestGateCmdHandler
{
private:

    void printTerminalPara(TerminalPara* p_paras);
    void printList(list <char*>* list);
    void testCmdHandleSingleExp(void);
    void testTansDataFromRule(void);
    void testCompareSrcInfo(void);
    void testGenDataFromText(void);
    void testInit(void);

    void testCmdHandle(void);
    void testGetTermParas(void);

    void testOutput(void);

public:
    void test(void);
};

#endif  //__TEST_GATECMDHANDLER_H__