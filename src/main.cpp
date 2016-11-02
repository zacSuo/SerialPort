//============================================================================
// Name        : main.cpp
// Author      : Lin, Kao-Yuan
// Version     :
// Copyright   : Your copyright notice
// Description : Demo Self Rx Tx by this SerialPort class
//============================================================================

#include <iostream>
#include <string>
#include "SerialPort.h"
using namespace std;

int  main()
{
	string strMsg = "!!!Hello World!!!";
	
	SerialPort* ptrSP = new SerialPort("/dev/ttyUSB0");

	if(ptrSP->isOpen())
	{

	}
	else
	{

	}

}
