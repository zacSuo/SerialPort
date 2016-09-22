#include <iostream>

#include "serialPort.h"

using namespace std;

void main(){

	serialPort serial;
	char buf = 'd';
	serial.sendMsg(&buf);
	serial.receiveMsg();
}