#ifndef __MSG_H__
#define __MSG_H__

#define MSG_SIZE 2
#define MESSAGE_FOR_SERVER 1

struct MsgBufRequest
{
	long type;
	struct DataForRequest
	{
		int pid;
		int num[MSG_SIZE];
	} data;
};

struct MsgBufAnswer
{
	long type;
	struct DataForAnswer
	{
		int answer;
	} data;
};

int createMsg(key_t key);
int connectToMsg(key_t key);

#endif /* __MSG_H__ */
