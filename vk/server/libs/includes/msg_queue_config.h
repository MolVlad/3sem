#ifndef __MSG_QUEUE_CONFIG_H
#define __MSG_QUEUE_CONFIG_H

#include"fifo_config.h"

#define MSG_SIZE 2
#define NAME_SIZE 10
#define MESSAGE_FOR_SERVER 1

struct MsgQueueBufTransmission
{
	long type;
	struct DataForTransmission
	{
		char privateFifo[PRIVATE_FIFO_NAME_SIZE];
	} data;
};

struct MsgQueueBufAnswer
{
	long type;
	struct DataForAnswer
	{
		Flag ACK;
		char privateFifo[PRIVATE_FIFO_NAME_SIZE];
	} data;
};

int createMsgQueue(key_t key);
int connectToMsgQueue(key_t key);

#endif /* __MSG_QUEUE_CONFIG_H */
