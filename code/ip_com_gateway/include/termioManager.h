/********************************************************************
 File: termioManager.h
 Function: Initial term io. Mainly read terminal paras from file.
 Author: Luobihe
 Date: 2015-12-31
*********************************************************************/

#ifndef __TERMIOMANAGER_H__
#define __TERMIOMANAGER_H__

#include <list>
using namespace std;

#define TERMINAL_IO_TYPE_TCP    1
#define TERMINAL_IO_TYPE_UDP    2
#define TERMINAL_IO_TYPE_RS232  3

#define TERMINAL_IO_DESC_TCP    "tcp"
#define TERMINAL_IO_DESC_UDP    "udp"
#define TERMINAL_IO_DESC_COM    "com"

#define FILE_DEV_PARAS  "dev_paras.dat"

//define io terminal
typedef struct {
    int type;
    char desc[256];
    char ip[64];       //wan parameters
    int port;          //wan parameters
    int boud;          //RS232 parameters
    int databits;      //RS232 parameters
    int stopbits;      //RS232 parameters
    int parity;        //RS232 parameters
} TerminalPara;

class CTermManager 
{
private:
    static CTermManager* m_instance;
    list <TerminalPara*> m_termParas;

private:
    CTermManager(void);

    int addTermFromText(const char* exp);
    int addTerm(TerminalPara* p_data);
    void removeAll(void);

    //initial, read port infomation from file
    int initList(void);

    //start listener
    int startListener(void);

public:    
    ~CTermManager(void);

    static CTermManager* getInstance(void);

    list <TerminalPara*>* getTermList(void) {
        return &m_termParas;
    }
    
    //读文件，生成端口信息
    int init(void);    

    //由输入描述，查找端口表，取得端口参数
    //0 -- success, -1 -- failed
    int getTermParas(const char* desc, TerminalPara* p_paras);
};

#endif  //__TERMIOMANAGER_H__