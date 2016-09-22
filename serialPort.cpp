#include "serialPort.h"


//设置串口中最基本的包括波特率设置，校验位和停止位设置。串口的设置主
//要是设置struct termios结构体的各成员值
//struct termio
//{	unsigned short  c_iflag;	/* 输入模式标志 */	
//	unsigned short  c_oflag;		/* 输出模式标志 */	
//	unsigned short  c_cflag;		/* 控制模式标志*/	
//	unsigned short  c_lflag;		/* local mode flags */	
//	unsigned char  c_line;		    /* line discipline */	
//	unsigned char  c_cc[NCC];    /* control characters */
//};

serialPort::serialPort(void)
{
	//打开串口
	nFd = OpenDevice(DEVICE);
	if(-1 == nFd)	return;
	//设置串口参数
	struct termios stOption;
	if(tcgetattr(nFd,&stOption) != 0) {        
		perror("tcgetattr error\n");  
		return;     
	} 	
	//波特率：115200
	cfsetispeed(&stOption, BAUDRATE);//115200
	cfsetospeed(&stOption, BAUDRATE);
	//数据位数：8位  
	stOption.c_cflag |= (CLOCAL|CREAD); //本地连接和接受使能
	stOption.c_cflag &= ~CSIZE;
	stOption.c_cflag |= CS8;
	//校验位：无
	stOption.c_cflag &= ~PARENB;   //Clear parity enable
	stOption.c_iflag &= ~INPCK;    // Enable parity checking
	//停止位：1
	stOption.c_cflag &= ~CSTOPB;
	//等待时间:1秒
	stOption.c_cc[VTIME]=10;	//指定读取第一个字符的等待时间，时间的单位为n*100ms
	//所要读取字符的最小数量	//如果设置VTIME=0，则无字符输入时read（）操作无限期的阻塞
	stOption.c_cc[VMIN]=1;	
	//处理未接收字符
	tcflush(nFd,TCIFLUSH);	//清空终端未完成的输入/输出请求及数据
	//激活新配置
	if( tcsetattr(nFd,TCSANOW,&stOption) != 0 )
	{
		perror("tcsetattr Error!\n");
	}
}


serialPort::~serialPort(void)
{
}

int serialPort::OpenDevice(char *Dev)
{
	//	 Open函数中除普通参数外，另有两个参数O_NOCTTY和O_NDELAY。
	//  O_NOCTTY: 通知linix系统，这个程序不会成为这个端口的控制终端。
	//  O_NDELAY: 通知linux系统不关心DCD信号线所处的状态（端口的另一端是否激活或者停止）。
	int	fd = open( Dev, O_RDWR|O_NOCTTY|O_NDELAY ); 
	if (-1 == fd)	
	{ 			
		perror("Can't Open Serial Port");
		return -1;		
	}	
	//恢复串口的状态为阻塞状态
	if( (fcntl(fd, F_SETFL, 0)) < 0 )
	{
		perror("Fcntl F_SETFL Error!\n");
		return -1;
	}
	//测试打开的文件描述府是否引用一个终端设备，以进一步确认串口是否正确打开
	//若为终端设备则返回1（真），否则返回0（假）
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
	printf("\nSend %d of %d\n", nCount, nLen);
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