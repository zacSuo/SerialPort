#include "SerialPort.h"
#include <iostream>
#include <string.h>
#include <unistd.h>
using namespace std;

SerialPort::SerialPort(string strDevice)
{
	this->nFd = OpenDevice(strDevice);
	if(-1 == this->nFd)	return;
	
	fcntl(this->nFd, F_SETFL, O_NONBLOCK);  // 設定為非阻塞（non-blocking）

    struct termios options;
    tcgetattr(this->nFd, &options);

    cfsetispeed(&options, BAUDRATE);
    cfsetospeed(&options, BAUDRATE);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_oflag &= ~OPOST;

    tcsetattr(this->nFd, TCSANOW, &options);
}

bool SerialPort::isOpen()
{
	return (-1 == this->nFd) ? false : true;
}

SerialPort::~SerialPort() {
	// TODO Auto-generated destructor stub
}

int SerialPort::OpenDevice(string strDevice)
{
	int	fd = open(strDevice.c_str(), O_RDWR|O_NOCTTY|O_NDELAY );
	if (-1 == fd)
	{
		perror("Can't Open Serial Port");
		return -1;
	}

	if( (fcntl(fd, F_SETFL, 0)) < 0 )
	{
		perror("Fcntl F_SETFL Error!\n");
		return -1;
	}

	if(isatty(STDIN_FILENO)==0)
	{
		printf("standard input is not a terminal device\n");
		return -1;
	}

	return fd;
}

int SerialPort::GetFileDescriptor()
{
	return this->nFd;
}

int SerialPort::Send(string strOutMsg)
{
	int nCount = write(this->nFd, strOutMsg.c_str(), strOutMsg.length());
	return nCount;
}

string SerialPort::Recv(void)
{
	char* strRxBuf = new char[16];
	//string strRxBuf(12,'\0');
	//cout << "strRxBuf.capacity()=" << strRxBuf.capacity() << endl;
	string strRxFullMsg = "";
		
	do{
		int nRead = read(this->nFd, strRxBuf, 16);
		if(0 >= nRead)break;
		strRxFullMsg.append(strRxBuf);
		memset(strRxBuf, '\0', 16);
		//strRxFullMsg += strRxBuf;
		
		/*
		cout << "strRxBuf.capacity()=" << strRxBuf.capacity() << endl;
		cout << strRxBuf << endl;
		//strRxBuf.clear();
		strRxBuf.replace(strRxBuf.begin(), strRxBuf.end(), "\0");
		cout << "strRxBuf.capacity()=" << strRxBuf.capacity() << endl;
		cout << strRxBuf << endl;
		*/
	}while(true);

	return strRxFullMsg;
}