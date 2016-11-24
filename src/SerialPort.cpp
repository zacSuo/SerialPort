#include "SerialPort.hpp"
#include <iostream>
#include <string.h>
#include <unistd.h>  
//#include <unistd.h> //For usleep()
using namespace std;

SerialPort::SerialPort(const string portName, int baudRate)
{
	printf("baudRate code = %d\n", baudRate);
	//this->nFd = OpenDevice(portName);
	this->nFd = open(portName.c_str(), O_RDWR|O_NOCTTY|O_NDELAY );
	if(-1 == this->nFd)
	{
		this->nFd = -1;
	}

	struct termios options = { 0 };   //宣告一個設定 comport 所需要的結構體 並且清空內部
	
	/* c_cflag 控制模式：
	 * CLOCAL:忽略任何modem status lines
	 * CREAD:啟動字元接收
	 * CS8:傳送或接收時，使用八個位元
	 */
	options.c_cflag = (baudRate | CLOCAL | CREAD | CS8); //依序,設定 baud rate,不改變 comport 擁有者, 接收致能, 8 data bits

	cfsetispeed( &options , baudRate );
	cfsetospeed( &options , baudRate );

	options.c_cc[ VTIME ] = 1;	//10 = 1秒,定義等待的時間，單位是百毫秒
	options.c_cc[ VMIN ] = 0;	//定義了要求等待的最小字節數,這個基本上給 0
	tcflush(this->nFd , TCIOFLUSH );	// 刷新和立刻寫進去fd

	if ( (tcsetattr( this->nFd , TCSANOW , &options )) == -1 )
	{ //寫回本機設備,TCSANOW >> 立刻改變數值
		this->nFd = -1;
	}
}

bool SerialPort::isOpen()
{//檢查port是否已經開啟
	return (-1 == this->nFd) ? false : true;
}

void SerialPort::Close()
{
	 close(this->nFd);
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

	return fd;
}

int SerialPort::GetFileDescriptor()
{//暫時開放，其實物件導向以後，這個資訊不應該public。不然等於是破壞port操作封裝性
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
	unsigned char strRxBuf[rxBufferSize]; //接收要用unsigned
	string strRxFullMsg = "";
	int nullRecvCounter=0;
	int nullFirstRecvCounter=0;

	do{
		memset(strRxBuf, 0, rxBufferSize); //清空緩衝
		int nRead = read(this->nFd, strRxBuf, rxBufferSize); //接收資料
		usleep(1000);
		//cout << "nRead= " << nRead << endl;
		if(0 >= nRead && !strRxFullMsg.empty())
		{//如果這一次沒有收到東西，且從未收過資料
			nullRecvCounter++;
			//printf("nullRecvCounter = %d\n",nullRecvCounter);
			if(nullRecvCounter>10)break;
		}
		else if(0 < nRead)
		{//如果有收到東西
			nullRecvCounter=0;//計數歸0
			strRxFullMsg.append((char*)strRxBuf, nRead);//加入字串才用signed
		}
		else
		{
			nullFirstRecvCounter++;
			//printf("nullFirstRecvCounter = %d\n",nullFirstRecvCounter);
		}

	}while(nullFirstRecvCounter+nullRecvCounter < 1000);//設定time out為1000mSec

	return strRxFullMsg;
}