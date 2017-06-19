/*
 * msg.cpp
 *
 *  Created on: May 26, 2017
 *      Author: pangwz
 */

#include "msg.h"

#include <iostream>

using namespace std;

msg* msg::pMsg = NULL;

msg::msg()
{
	msgOut = "msg to print out";
}

msg::~msg()
{
	msgOut = NULL;
}

msg* msg::getInstance()
{
	if (NULL == pMsg){
		pMsg = new msg();
	}

	return pMsg;
}

char msg::delInstance()
{
	if (!pMsg){
		delete pMsg;
	}

	pMsg = NULL;
}

int msg::printMsg()
{
	cout << msgOut << endl;
}
