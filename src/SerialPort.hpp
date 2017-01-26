#ifndef _SERIALPORT_HPP_
#define _SERIALPORT_HPP_

#include     <stdio.h> 
#include     <stdlib.h> 
#include     <sys/types.h>
#include     <sys/stat.h>
#include     <fcntl.h> 
#include     <termios.h> 
#include     <errno.h>
#include	 <string>

using namespace std;

typedef int BaudRate;

class SerialPort {
public:
	SerialPort(const string portName, BaudRate baudRate);
	virtual ~SerialPort();
	bool isOpen();
	void Close();
	
	int Send(string);
	int Send(unsigned char*, size_t);
	string Recv(void);

	static const BaudRate BR115200 = 0010002;
	static const BaudRate  BR19200 = 0000016;
	static const BaudRate   BR9600 = 0000015;

	int GetFileDescriptor();
private:
	
	int OpenDevice(string);
	int nFd;
};

#endif /* SERIALPORT_H_ */
