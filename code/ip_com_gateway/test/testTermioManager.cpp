#include <stdio.h>
#include "testTermioManager.h"

void CTestTermioManager::printList(list <TerminalPara*>* p_list)
{
    list <TerminalPara*>::iterator pos;
    TerminalPara* p_data = NULL;
    printf("size = %d\n", p_list->size());

    for (pos = p_list->begin(); pos != p_list->end(); pos++) {
        p_data = *pos;
        if (p_data->type == TERMINAL_IO_TYPE_TCP) {
            printf("desc = %s, ip = %s, port = %d\n", p_data->desc, p_data->ip, p_data->port);
        }
        else {
            printf("desc = %s, bord = %d, databits = %d, stopbits = %d, parity = %d\n",
                p_data->desc, p_data->boud, p_data->databits, 
                p_data->stopbits, p_data->parity);
        }
    }
}

void CTestTermioManager::testInit(void)
{
    printf("======================test init=====================\n");
    CTermManager* p_data = CTermManager::getInstance();
    list <TerminalPara*>* p_list = p_data->getTermList();
    printf("Before init\n");
    printList(p_list);

    p_data->init();
    printf("After init\n");
    printList(p_list);
}

void CTestTermioManager::testGetTermParas(void)
{
    TerminalPara paras = {0};

    printf("======================test GetTermParas=====================\n");
    CTermManager* p_manager = CTermManager::getInstance();
    p_manager->init();

    list <TerminalPara*>* p_list = p_manager->getTermList();

    printf("before get\n");
    printList(p_list);

    p_manager->getTermParas("Com2", &paras);
    TerminalPara* p_data = &paras;

    if (p_data->type == TERMINAL_IO_TYPE_TCP) {
        printf("desc = %s, ip = %s, port = %d\n", p_data->desc, p_data->ip, p_data->port);
    }
    else {
        printf("desc = %s, bord = %d, databits = %d, stopbits = %d, parity = %d\n",
            p_data->desc, p_data->boud, p_data->databits, 
            p_data->stopbits, p_data->parity);
    }
}

void CTestTermioManager::testRemoveAll(void)
{
#if 0
    printf("======================test remove all=====================\n");
    list <TerminalPara*>* p_list = m_test.getTermList();
    printf("Before remove all\n");
    printList(p_list);

    m_test.removeAll();
    printf("After remove all\n");
    printList(p_list);
#endif
}

void CTestTermioManager::test(void)
{
    testInit();
    //testRemoveAll();
    //testGetTermParas();
}