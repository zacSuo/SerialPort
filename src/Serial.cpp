//============================================================================
// Name        : Serial.cpp
// Author      : vejoe1
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "serialPort.h"

using namespace std;

int  main(){

	serialPort serial;
	char* buf = "!!!Hello World!!!";
	serial.sendMsg(buf);
	serial.receiveMsg();

	cout << buf << endl; // prints !!!Hello World!!!
	return 0;
}
