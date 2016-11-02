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

class SerialPort {
public:
	SerialPort(string);
	virtual ~SerialPort();
	bool isOpen();
	
	int Send(string);
	int Send(unsigned char*, size_t);
	string Recv(void);

private:
	int GetFileDescriptor();
	int OpenDevice(string);
	int nFd;
};

#endif /* SERIALPORT_H_ */
