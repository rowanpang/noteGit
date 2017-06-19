/*
 * msg.h
 *
 *  Created on: May 26, 2017
 *      Author: pangwz
 */

#ifndef MSG_H_
#define MSG_H_


class msg
{
private:
	msg();
	~msg();

public:
	static msg* getInstance();
	static char delInstance();
	int printMsg();

private:
	static msg* pMsg;
	char *msgOut;
};



#endif /* MSG_H_ */
