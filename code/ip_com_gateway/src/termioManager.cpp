/********************************************************************
 File: termioManager.cpp
 Function: Initial term io.
 Author: Luobihe
 Date: 2015-12-31
*********************************************************************/

#include <stdio.h>
#include <string.h>
#include "termioManager.h"
#include "tcplistener.h"

CTermManager* CTermManager::m_instance = NULL;

CTermManager* CTermManager::getInstance(void)
{
    if (!m_instance)
        m_instance = new CTermManager();

    return m_instance;
}

CTermManager::CTermManager(void)
{
}

CTermManager::~CTermManager(void)
{
    removeAll();
}

int CTermManager::init(void)
{
    if (initList())
        return -1;

    return startListener();    
}

//由输入描述，查找端口表，取得端口参数
//0 -- success, -1 -- failed
int CTermManager::getTermParas(const char* desc, TerminalPara* p_paras)
{
    TerminalPara* p_data = NULL;
    list <TerminalPara*> ::iterator pos = m_termParas.begin();
    list <TerminalPara*> ::iterator last = m_termParas.end();
    for (; pos != last; pos++) {
        p_data = *pos;
        if (stricmp(p_data->desc, desc) == 0) {
            memcpy(p_paras, p_data, sizeof(TerminalPara));
            return 0;
        }
    }

    return -1;
}

int CTermManager::initList(void)
{
    TerminalPara data = {0};
    
    char fileName[256] = {0};
    char fileDir[256] = {0};
    char processName[256] = {0};
    FILE* p_file = NULL;
    char szExp[1024] = {0};   

    removeAll();
 
    if (getExePath(fileDir, processName, 256))
        return -1;   

    sprintf(fileName, "%s/%s", fileDir, FILE_DEV_PARAS);

    p_file = fopen(fileName, "r");
    if (!p_file)
        return -1;

    while (fgets(szExp, 1024, p_file)) {
        addTermFromText(szExp);
    }

    fclose(p_file);
    
    return 0;
}

int CTermManager::startListener(void)
{
    TerminalPara* p_data = NULL;
    list <TerminalPara*> ::iterator pos = m_termParas.begin();
    list <TerminalPara*> ::iterator last = m_termParas.end();
    for (; pos != last; pos++) {
        p_data = *pos;
        if (stricmp(p_data->desc, "tcp") == 0) {
            startTcpListen(p_data->ip, p_data->port);
        }
    }

    return 0;
}

int CTermManager::addTermFromText(const char* exp)
{
    TerminalPara data = {0};
    char parity = 0;

    if (strncmp(exp, TERMINAL_IO_DESC_TCP, strlen(TERMINAL_IO_DESC_TCP)) == 0) {
        //get port, ip
        data.type = TERMINAL_IO_TYPE_TCP;
        strcpy(data.desc, TERMINAL_IO_DESC_TCP);
        strcpy(data.ip, "192.168.1.158");
#ifndef _WIN32
        getLocalIp(data.ip);
#endif    
        sscanf(exp, "%*[^:]%*c%d", &data.port);
        return addTerm(&data);
    }
    else if (strncmp(exp, TERMINAL_IO_DESC_COM, strlen(TERMINAL_IO_DESC_COM)) == 0) {
        //get com paras
        data.type = TERMINAL_IO_TYPE_RS232;
        sscanf(exp, "%[^:]%*c%d%*c%d%*c%d%*c%c", 
            data.desc, &data.boud, &data.databits, 
            &data.stopbits, &parity);

        data.parity = parity;

        return addTerm(&data);
    }

    return -1;
}

int CTermManager::addTerm(TerminalPara* p_data)
{
    TerminalPara* data = new TerminalPara;
    if (!data)
        return -1;

    memcpy(data, p_data, sizeof(TerminalPara));

    m_termParas.push_back(data);

    return 0;
}

void CTermManager::removeAll(void)
{
    TerminalPara* p_data = NULL;
    list <TerminalPara*> ::iterator pos = m_termParas.begin();
    list <TerminalPara*> ::iterator last = m_termParas.end();

    while (pos != last) {
        p_data = *pos;

        pos = m_termParas.erase(pos);
        delete p_data;
        p_data = NULL;
    }
}
