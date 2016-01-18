#include <stdio.h>
#include "testTcpListener.h"

void CTestTcpListener::printList(list <TcpPeer*>* p_list)
{
    list <TcpPeer*>::iterator pos;
    printf("size = %d\n", p_list->size());

    for (pos = p_list->begin(); pos != p_list->end(); pos++) {
        printf("fd = %d, ip = %s, port = %d\n",
            (*pos)->fd, (*pos)->ip, (*pos)->port);
    }
}

void CTestTcpListener::printList(void)
{
    list <TcpPeer*>::iterator pos;
    list <TcpPeer*>* p_list = m_inst.getList();
    printf("size = %d\n", p_list->size());

    for (pos = p_list->begin(); pos != p_list->end(); pos++) {
        printf("fd = %d, ip = %s, port = %d\n",
            (*pos)->fd, (*pos)->ip, (*pos)->port);
    }
}

void CTestTcpListener::testAddClient(void)
{
    printf("======================test add client=====================\n");

    TcpPeer peer = {0};

    printf("Before add\n");
    printList();

    for (int i = 0; i < 3; i++) {
        peer.fd = i + 1;
        sprintf(peer.ip, "192.168.1.%d", i + 100);
        peer.port = 1000 + i;
        m_inst.addClient(&peer);
    }

    printf("After add\n");
    printList();
}

void CTestTcpListener::testRemoveClient(void)
{
    printf("======================test remove client=====================\n");

    printf("Before remove\n");
    printList();

    m_inst.removeClient(1);

    printf("After remove\n");
    printList();
}

void CTestTcpListener::testRemoveAll(void)
{
    printf("======================test remove all=====================\n");

    printf("Before remove all\n");
    printList();

    m_inst.removeAllClient();

    printf("After remove all\n");
    printList();
}

void CTestTcpListener::testCopyClientList(void)
{
    printf("======================test CopyClientList=====================\n");
    list <TcpPeer*> p_list;

    printf("Before remove all\n");
    printList(&p_list);

    m_inst.copyClientList(&p_list);
    
    printf("After remove all\n");
    printList(&p_list);
    
}

void CTestTcpListener::test(void)
{
    testAddClient();

    testRemoveClient();

    testCopyClientList();
}