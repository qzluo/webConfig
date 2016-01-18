/********************************************************************
 File: serialCom.h
 Function: serial communication function
 Author: Luobihe
 Date: 2016-1-6
*********************************************************************/

#ifndef __SERIALCOM_H__
#define __SERIALCOM_H__

//设置串口通信速率
int set_speed(int fd, int speed);

//设置串口数据位，停止位和效验位
int set_Parity(int fd, int databits, int stopbits, int parity);

#endif  //__SERIALCOM_H__