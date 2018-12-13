#ifndef __MSG_H__
#define __MSG_H__

#define MSG_SIZE 2
#define NAME_SIZE 10
#define MESSAGE_FOR_SERVER 1

struct MsgBufTransmission
{
	long type;
	struct DataForRequest
	{
		char fifo[NAME_SIZE];
	} data;
};

struct MsgBufAnswer
{
	long type;
	struct DataForAnswer
	{
		Flag ACK;
	} data;
};

int createMsg(key_t key);
int connectToMsg(key_t key);

#endif /* __MSG_H__ */
