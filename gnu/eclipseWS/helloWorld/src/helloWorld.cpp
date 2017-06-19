//============================================================================
// Name        : helloWorld.cpp
// Author      : pangwz
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "msg.h"

using namespace std;

int main() {
	cout << "!!!Hello World  asdf!!!" << endl; // prints !!!Hello World!!!

	msg::getInstance()->printMsg();

//	msg* myMsg = new msg(); 			//error! for private construct

	msg::delInstance();
	return 0;
}
