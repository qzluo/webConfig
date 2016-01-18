#ifndef __TEST_TCPLISTENER_H__
#define __TEST_TCPLISTENER_H__

#include "tcplistener.h"


class CTestTcpListener
{
private:
    CTcpSocket m_inst;

private:
    void printList(list <TcpPeer*>* p_list);
    void printList(void);
    void testAddClient(void);
    void testRemoveClient(void);
    void testRemoveAll(void);
    void testCopyClientList(void);

public:
    void test(void);
};

#endif  //__TEST_TCPLISTENER_H__