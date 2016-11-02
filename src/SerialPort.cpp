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
{//檢查port是否已經開啟
	return (-1 == this->nFd) ? false : true;
}

SerialPort::~SerialPort() {
	// TODO Auto-generated destructor stub
	//應該有些什麼東西要手動free掉？ 目前看起來沒有
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
{//暫時開放，其實物件導向以後，這個資訊不應該public。不然等於是破壞terminal操作封裝性
	return this->nFd;
}

//寫入字串資訊
int SerialPort::Send(string strOutMsg)
{
	int nCount = write(this->nFd, strOutMsg.c_str(), strOutMsg.length());
	return nCount;
}

//寫入二進位資訊，所以必須自己給長度。
int SerialPort::Send(unsigned char* strOutMsg, size_t nbyte)
{
	int nCount = write(this->nFd, strOutMsg, nbyte);
	return nCount;
}

string SerialPort::Recv(void)
{//接收資訊
	const static size_t rxBufferSize = 12;
	unsigned char strRxBuf[rxBufferSize];
	string strRxFullMsg = "";

	do{
		memset(strRxBuf, 0, rxBufferSize); //清空緩衝
		int nRead = read(this->nFd, strRxBuf, rxBufferSize); //接收資料
		if(-1 == nRead)break;

		char* temp = (char*)strRxBuf;
		strRxFullMsg.append(temp);
	}while(true);//要設定time out

	return strRxFullMsg;
}