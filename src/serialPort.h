/*
 * serialPort.h
 *
 *  Created on: Sep 22, 2016
 *      Author: vejoe
 */

#ifndef SERIALPORT_H_
#define SERIALPORT_H_

#include     <stdio.h>      /*±ê×ŒÊäÈëÊä³ö¶šÒå*/
#include     <stdlib.h>     /*±ê×Œº¯Êý¿â¶šÒå*/
#include     <unistd.h>     /*Unix ±ê×Œº¯Êý¶šÒå*/
#include     <sys/types.h>
#include     <sys/stat.h>
#include     <fcntl.h>      /*ÎÄŒþ¿ØÖÆ¶šÒå*/
#include     <termios.h>    /*PPSIX ÖÕ¶Ë¿ØÖÆ¶šÒå*/
#include     <errno.h>      /*ŽíÎóºÅ¶šÒå*/
#include	 <string.h>

#define BAUDRATE B115200 //Baud rate : 115200
#define DEVICE "/dev/ttyS3"
#define SIZE 1024

class serialPort {
public:
	serialPort();
	virtual ~serialPort();

	void sendMsg(char *);
	void receiveMsg(void);

private:
	int OpenDevice(char *);
	int nFd;
};

#endif /* SERIALPORT_H_ */
