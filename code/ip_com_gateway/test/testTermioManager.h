#ifndef __TEST_TERMIOMANAGER_H__
#define __TEST_TERMIOMANAGER_H__

#include "termioManager.h"

class CTestTermioManager
{
private:
    void printList(list <TerminalPara*>* p_list);
    void testGetTermParas(void);

    void testInit(void);
    void testRemoveAll(void);

public:
    void test(void);
};

#endif  //__TEST_TERMIOMANAGER_H__