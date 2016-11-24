//============================================================================
// Name        : main.cpp
// Author      : Lin, Kao-Yuan
// Version     :
// Copyright   : Your copyright notice
// Description : Demo Self Rx Tx by this SerialPort class
//============================================================================

#include <iostream>
#include <string>
#include <unistd.h> //For usleep()
#include "SerialPort.hpp"
using namespace std;

int main(int argc, char *argv[])
{
	argc = 2 ; argv[1] = (char*)"ttyUSB0"; //dummy input

	string strModemDevice = "/dev/";
	while(NULL == argv[1])
	{
		char cInput;
		cout << "欠缺參數(ttyS0 或 ttyUSB0 之類的)" << endl;
		cout << "要幫懶惰的你補上\"ttyUSB0\"嗎？" << endl;
		cout << "[Y/n] "; cin >> cInput;

		if(toupper(cInput) == 'N')
		{
			cout << "結束程式" << endl;
			return 0;//跳出程式
		}
		else if(toupper(cInput) == 'Y')
		{
			argc = 2; argv[1] = (char*)"ttyUSB0"; //dummy input
			break;
		}
		system("clear");
	}

	cout << "argv[1]: " << argv[1] << endl; 
	strModemDevice.append(argv[1]); //在string後面加上一個char*字串
	cout << "strModemDevice = " << strModemDevice << endl;//顯示總字串

	//權限取得
	string strPermissionGetCommand = "sudo chmod 666 " + strModemDevice;
	system(strPermissionGetCommand.c_str());

	//建立物件
	SerialPort* serialPort = new SerialPort(strModemDevice, SerialPort::BR115200);

	if(serialPort->isOpen())
	{//開檔成功
		cout << "Pass!" << endl;
		
		//把 Hex String 轉換到 Byte Array
		string strSendMsg = "HelloWorld!";
		
		//送出 Byte Array 資料
		cout << "strSendMsg = " << strSendMsg << endl;
		cout << "send return = " << serialPort->Send(strSendMsg) << endl;

		//接收字串
		string strRx = serialPort->Recv();
		cout << "Serial Port Receive  : " << strRx << endl;
	}
	else
	{//開檔失敗
		cout << "Fail!" << endl; 
	}

	serialPort->Close();
	delete(serialPort);
}
