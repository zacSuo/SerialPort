#pragma once

#include     <stdio.h>      /*标准输入输出定义*/
#include     <stdlib.h>     /*标准函数库定义*/
#include     <unistd.h>     /*Unix 标准函数定义*/
#include     <sys/types.h>  
#include     <sys/stat.h>   
#include     <fcntl.h>      /*文件控制定义*/
#include     <termios.h>    /*PPSIX 终端控制定义*/
#include     <errno.h>      /*错误号定义*/
#include	 <string.h>

#define BAUDRATE B115200 //Baud rate : 115200
#define DEVICE "/dev/ttyAMA0"
#define SIZE 1024

class serialPort
{
public:
	serialPort(void);
	~serialPort(void);
	
	//收发消息
	void sendMsg(char *);
	void receiveMsg(void);

private:
	int OpenDevice(char *);
	int nFd;

};

