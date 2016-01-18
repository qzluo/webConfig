/********************************************************************
 File: tcplistener.h
 Function: tcp 监听app消息
 Author: Luobihe
 Date: 2015-7-8
********************************************************************/

#ifndef __TCPLISTENER_H__
#define __TCPLISTENER_H__

#include <list>
using namespace std;

#include "platcomm.h"
#include "cmdParser.h"

typedef struct {
    char ip[20];
    int port;
} ListenParas;

typedef struct {
    int fd;
    char ip[20];
    int port;
} TcpPeer;

//启动tcp监听
int startTcpListen(const char* p_ip, int port); 

//tcp socket
class CTcpSocket
{
private:
    int m_sfd;                //server socket
    list <TcpPeer*> m_listClient;  //client list

    EasyMutex m_MutexLock;    //lock

    CCmdParser* m_pParser;    //command parser

private:
public:

    //add client socket to list
    void addClient(TcpPeer* p_client);

    //copy the client list
    void copyClientList(list <TcpPeer*> * p_listDesc);

    //remove a client from list
    void removeClient(int cfd);

    //remove all client
    void removeAllClient(void);

public:
    CTcpSocket(void);
    ~CTcpSocket(void);

    list <TcpPeer*>* getList(void) { return &m_listClient; }

    //启动监听连接
    int startListen(const char* szIpAddr, int iPort);
    void clear(void);    

    //thread function to handle client communication
    static void* clientHandle(void* arg);

};

#endif  //__TCPLISTENER_H__