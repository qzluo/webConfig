/********************************************************************
 File: tcplistener.h
 Function: tcp 监听app消息
 Author: Luobihe
 Date: 2015-7-8
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tcplistener.h"
#include "gateCmdHandler.h"

#ifdef _WIN32

#include <Windows.h>
#include <winsock.h>

#else

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>

#endif


//监听线程函数  
static void* TcpListenThreadFun(void* pM)  
{  
    ListenParas* p_data = (ListenParas*)pM;

    CTcpSocket tcpServer;

    printf("Start Tcp server. ip = %s, port = %d\n",
        p_data->ip, p_data->port);

    if (tcpServer.startListen(p_data->ip, p_data->port) == -1)               
        return NULL;

    tcpServer.clear();

    return 0;
}

/*-------------------------------------------------------------------
 Function: startTcpListen(void);
           启动tcp监听线程
 Parameters:
 Return: 0 -- success
         -1 -- failed
-------------------------------------------------------------------*/
int startTcpListen(const char* p_ip, int port)
{
    static ListenParas paras = {0};
    strcpy(paras.ip, p_ip);
    paras.port = port;

    return easyCreateThread(TcpListenThreadFun, &paras);
}

/////////////////////////////////////////////////////////////////////
//CTcpSocket

CTcpSocket::CTcpSocket(void)
{
    m_sfd = -1;
    m_pParser = NULL;

    m_MutexLock.init();
}

CTcpSocket::~CTcpSocket(void)
{
    clear();

    m_MutexLock.destroy();
}

void CTcpSocket::clear(void)
{
    if (m_sfd) {
        removeAllClient();

#ifdef _WIN32
        closesocket(m_sfd);
#else
        close(m_sfd);
#endif

        m_sfd = -1;
    }

    if (m_pParser) {
        delete m_pParser;
        m_pParser = NULL;
    }
}

//启动监听
int CTcpSocket::startListen(const char* szIpAddr, int iPort)
{
    if (initTcpServer(szIpAddr, iPort, &m_sfd) == -1)
        return -1;

    if (m_pParser) {
        delete m_pParser;
        m_pParser = NULL;
    }

    if (easyCreateThread(CTcpSocket::clientHandle, this) == -1)
        return NULL;

    while(1) {
        int cfd = -1;
        struct sockaddr_in peeraddr = {0};
        int addrlen = sizeof(peeraddr);
        TcpPeer peer = {0};

#ifdef _WIN32
        cfd = accept(m_sfd, (struct sockaddr*)&peeraddr, &addrlen);
#else
        cfd = accept(m_sfd, (struct sockaddr*)&peeraddr, (socklen_t*)&addrlen);
#endif
        printf("Accepted client:%s:%d\n", 
            inet_ntoa(peeraddr.sin_addr), 
            ntohs(peeraddr.sin_port));

        peer.fd = cfd;
        strcpy(peer.ip, inet_ntoa(peeraddr.sin_addr));
        peer.port = ntohs(peeraddr.sin_port);

        addClient(&peer);
    }    

    return 0;
}

//客户端通信线程
void* CTcpSocket::clientHandle(void* arg)
{
    printf("Client handle thread\n");

    fd_set fdread;        
    int fd_temp = -1;
    CTcpSocket* p_tcpSock = (CTcpSocket*)arg;
    struct timeval tv = {1, 0};               
    int ret = 0;   
    char szMessage[1024] = {0}; 
    
    list <TcpPeer*> listTemp;
    list <TcpPeer*>::iterator pos;    

    while (1) {                 
        //initial read set
        p_tcpSock->copyClientList(&listTemp);

        FD_ZERO(&fdread);

        FD_SET(p_tcpSock->m_sfd, &fdread);
        int nfds = p_tcpSock->m_sfd;        
        for (pos = listTemp.begin(); pos != listTemp.end(); pos++) {
            fd_temp = (*pos)->fd;
            FD_SET(fd_temp, &fdread); 
            if (fd_temp > nfds)
                nfds = fd_temp;
        }    

        tv.tv_sec = 1;
        tv.tv_usec = 0;
        ret = select(nfds + 1, &fdread, NULL, NULL, &tv); 

        if (ret == 0)  // Time expired
            continue;

        for (pos = listTemp.begin(); pos != listTemp.end(); pos++) {    
            int cfd = (*pos)->fd;
            if (FD_ISSET(cfd, &fdread)) {
                ret = recv(cfd, szMessage, 1024, 0);

#ifdef _WIN32

                if (ret == 0 || (ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)) { // Client socket closed   
#else

                if (ret <= 0) {
                    if ((ret < 0) && (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN))
                        continue;
#endif                         

                    printf("Client socket %d closed.\n", cfd);                
                    p_tcpSock->removeClient(cfd);
                }                       
                else {
                    //解析收到的命令并处理
                    char szSrcInfo[256] = {0};
                    CGateCmdHandler* p_cmdHandler = CGateCmdHandler::getInstance();
                    if (!p_cmdHandler)
                        continue;

                    sprintf(szSrcInfo, "tcp %s", (*pos)->ip);
                    p_cmdHandler->cmdHandle(szSrcInfo, szMessage, ret);
                }
            }
        }
    }

    return NULL;
}

void CTcpSocket::addClient(TcpPeer* p_client)
{
    TcpPeer* client = new TcpPeer;
    memcpy(client, p_client, sizeof(TcpPeer));

    m_MutexLock.lock();
    m_listClient.push_back(client);
    m_MutexLock.unlock();
}

void CTcpSocket::copyClientList(list <TcpPeer*> * p_listDesc)
{
    p_listDesc->clear();

    m_MutexLock.lock();
    p_listDesc->assign(m_listClient.begin(), m_listClient.end());
    m_MutexLock.unlock();
}

void CTcpSocket::removeClient(int cfd)
{
    TcpPeer* p_client = NULL;

    m_MutexLock.lock();

    list <TcpPeer*>::iterator pos = m_listClient.begin();
    list <TcpPeer*>::iterator last = m_listClient.end();

    while (pos != last) {
        p_client = *pos;
        if (p_client->fd == cfd) {
#ifdef _WIN32
            closesocket(cfd); 
#else
            close(cfd);
#endif
            pos = m_listClient.erase(pos);
            delete p_client;
            p_client = NULL;
        }
        else
            pos++;        
    }

    m_MutexLock.unlock();
}

void CTcpSocket::removeAllClient(void)
{
    TcpPeer* p_client = NULL;

    m_MutexLock.lock();

    list <TcpPeer*>::iterator pos = m_listClient.begin();
    list <TcpPeer*>::iterator last = m_listClient.end();

    while (pos != last) {
        p_client = *pos;

#ifdef _WIN32
        closesocket(p_client->fd); 
#else
        close(p_client->fd);
#endif
        pos = m_listClient.erase(pos);     
        delete p_client;
        p_client = NULL;
    }

    m_MutexLock.unlock();
}