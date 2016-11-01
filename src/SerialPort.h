#ifndef SERIALPORT_H_
#define SERIALPORT_H_

#include     <stdio.h> 
#include     <stdlib.h> 
#include     <unistd.h>  
#include     <sys/types.h>
#include     <sys/stat.h>
#include     <fcntl.h> 
#include     <termios.h> 
#include     <errno.h>
#include	 <string>
using namespace std;

#define BAUDRATE B19200 //Baud rate : 19200
#define SIZE 1024

class SerialPort {
public:
	SerialPort(string strDevice);
	virtual ~SerialPort();
	bool isOpen();
	int GetFileDescriptor();
	int Send(string strOutMsg);
	string Recv(void);

private:
	int OpenDevice(string strDevice);
	int nFd;
};

#endif /* SERIALPORT_H_ */
