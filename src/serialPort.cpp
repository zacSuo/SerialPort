/*
 * serialPort.cpp
 *
 *  Created on: Sep 22, 2016
 *      Author: vejoe
 */

#include "serialPort.h"

serialPort::serialPort() {
	//Žò¿ªŽ®¿Ú
	nFd = OpenDevice(DEVICE);
	if(-1 == nFd)	return;
	//ÉèÖÃŽ®¿Ú²ÎÊý
	struct termios stOption;
	if(tcgetattr(nFd,&stOption) != 0) {
		perror("tcgetattr error\n");
		return;
	}
	//²šÌØÂÊ£º115200
	cfsetispeed(&stOption, BAUDRATE);//115200
	cfsetospeed(&stOption, BAUDRATE);
	//ÊýŸÝÎ»Êý£º8Î»
	stOption.c_cflag |= (CLOCAL|CREAD); //±ŸµØÁ¬œÓºÍœÓÊÜÊ¹ÄÜ
	stOption.c_cflag &= ~CSIZE;
	stOption.c_cflag |= CS8;
	//Ð£ÑéÎ»£ºÎÞ
	stOption.c_cflag &= ~PARENB;   //Clear parity enable
	stOption.c_iflag &= ~INPCK;    // Enable parity checking
	//Í£Ö¹Î»£º1
	stOption.c_cflag &= ~CSTOPB;
	//µÈŽýÊ±Œä:1Ãë
	stOption.c_cc[VTIME]=10;	//Öž¶š¶ÁÈ¡µÚÒ»žö×Ö·ûµÄµÈŽýÊ±Œä£¬Ê±ŒäµÄµ¥Î»Îªn*100ms
	//ËùÒª¶ÁÈ¡×Ö·ûµÄ×îÐ¡ÊýÁ¿	//Èç¹ûÉèÖÃVTIME=0£¬ÔòÎÞ×Ö·ûÊäÈëÊ±read£š£©²Ù×÷ÎÞÏÞÆÚµÄ×èÈû
	stOption.c_cc[VMIN]=1;
	//ŽŠÀíÎŽœÓÊÕ×Ö·û
	tcflush(nFd,TCIFLUSH);	//Çå¿ÕÖÕ¶ËÎŽÍê³ÉµÄÊäÈë/Êä³öÇëÇóŒ°ÊýŸÝ
	//Œ€»îÐÂÅäÖÃ
	if( tcsetattr(nFd,TCSANOW,&stOption) != 0 )
	{
		perror("tcsetattr Error!\n");
	}

}

serialPort::~serialPort() {
	// TODO Auto-generated destructor stub
}
int serialPort::OpenDevice(char *Dev)
{
	//	 Openº¯ÊýÖÐ³ýÆÕÍš²ÎÊýÍâ£¬ÁíÓÐÁœžö²ÎÊýO_NOCTTYºÍO_NDELAY¡£
	//  O_NOCTTY: ÍšÖªlinixÏµÍ³£¬Õâžö³ÌÐò²»»á³ÉÎªÕâžö¶Ë¿ÚµÄ¿ØÖÆÖÕ¶Ë¡£
	//  O_NDELAY: ÍšÖªlinuxÏµÍ³²»¹ØÐÄDCDÐÅºÅÏßËùŽŠµÄ×ŽÌ¬£š¶Ë¿ÚµÄÁíÒ»¶ËÊÇ·ñŒ€»î»òÕßÍ£Ö¹£©¡£
	int	fd = open( Dev, O_RDWR|O_NOCTTY|O_NDELAY );
	if (-1 == fd)
	{
		perror("Can't Open Serial Port");
		return -1;
	}
	//»ÖžŽŽ®¿ÚµÄ×ŽÌ¬Îª×èÈû×ŽÌ¬
	if( (fcntl(fd, F_SETFL, 0)) < 0 )
	{
		perror("Fcntl F_SETFL Error!\n");
		return -1;
	}
	//²âÊÔŽò¿ªµÄÎÄŒþÃèÊöž®ÊÇ·ñÒýÓÃÒ»žöÖÕ¶ËÉè±ž£¬ÒÔœøÒ»²œÈ·ÈÏŽ®¿ÚÊÇ·ñÕýÈ·Žò¿ª
	//ÈôÎªÖÕ¶ËÉè±žÔò·µ»Ø1£šÕæ£©£¬·ñÔò·µ»Ø0£šŒÙ£©
	if(isatty(STDIN_FILENO)==0)
	{
		printf("standard input is not a terminal device\n");
		return -1;
	}

	return fd;
}

void serialPort::sendMsg(char * msg){
	int nLen = strlen(msg);
	int nCount = write(nFd,msg,nLen);
	printf("\nSend %d of %d: %s\n", nCount, nLen, msg);
}

void serialPort::receiveMsg(void){
	int nread;
	char * msg = new char[512];
	while(true)
	{
		while((nread = read(nFd,msg,512)) > 0)
		{
			printf("\nRecieve %d\n", nread);
			msg[nread+1] = '\0';
			printf("\n%s",msg);
		}
	}
}
