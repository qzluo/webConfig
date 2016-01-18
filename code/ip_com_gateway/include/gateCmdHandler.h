/********************************************************************
 File: gateCmdHandler.h
 Function: Transfer the source data to the target data. 
           Output the target data.
 Author: Luobihe
 Date: 2016-1-4
*********************************************************************/

#ifndef __GATECMDHANDLER_H__
#define __GATECMDHANDLER_H__

#include <list>
using namespace std;

#include "termioManager.h"

#define MAX_DATA_LENGTH   258
#define MAX_INFO_LENGTH   64

#define FILE_DATA_TRANS   "data_trans.dat"

//初始化，读配置文件
int initObj(void);

class CGateCmdHandler
{
private:
    static CGateCmdHandler* m_pInst;
    list <char*> m_listTransExp;

private:
    CGateCmdHandler(void); 
    void clearList(void);

    //单表达式命令处理，包括数据转换及命令执行
    //0 -- success, -1 -- failed
    int cmdHandleSingleExp(const char* expression, 
        const char* srcInfo, char* in_data, int in_datalen);

    //根据表达式，由输入源信息及输入数据产生输出端口信息及输出数据
    int tansDataFromRule(const char* expression, const char* srcInfo,
        char* in_data, int in_datalen, char* tarInfo, char* out_data);

    //命令执行
    int putoutData(const char* tarInfo, char* out_data, int out_datalen);   

    int compareSrcInfo(const char* info_from_src, const char* info_from_exp);
    int compareInData(char* in_data, int in_datalen, const char* indata_from_exp);

    //从16进制格式的文本产生数据
    //文本格式为16进制，每字节占2位，且以0x起始，中间以空格分隔
    int genDataFromText(const char* data_text, char* out_data, int* datalen);

    int addExpression(const char* exp);

    //由端口字符标识，取得接口参数 
    //0 -- success, -1 -- failed
    int getTermParas(const char* tarInfo, TerminalPara* p_paras);     

public:
    ~CGateCmdHandler(void);    

    static CGateCmdHandler* getInstance(void);

    //init 初始化，读数据文件
    int init(void);

    //命令处理
    void cmdHandle(const char* srcInfo, char* in_data, int in_datalen);

    list <char*>* getList(void) {
        return &m_listTransExp;
    }    
};

#endif  //__GATECMDHANDLER_H__
