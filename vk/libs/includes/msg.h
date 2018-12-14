#ifndef __MSG_H__
#define __MSG_H__

#include"config_serv_hand.h"

#define MSG_SIZE 2
#define NAME_SIZE 10
#define MESSAGE_FOR_SERVER 1

struct MsgBufTransmission
{
	long type;
	struct DataForTransmission
	{
		char privateFifo[PRIVATE_FIFO_NAME_SIZE];
	} data;
};

struct MsgBufAnswer
{
	long type;
	struct DataForAnswer
	{
		Flag ACK;
		char privateFifo[PRIVATE_FIFO_NAME_SIZE];
	} data;
};

int createMsg(key_t key);
int connectToMsg(key_t key);

#endif /* __MSG_H__ */
