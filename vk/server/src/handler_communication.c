#include"libs.h"
#include"config.h"
#include"general_config.h"
#include"config_serv_hand.h"
#include"my_string.h"
#include"handler_communication.h"
#include"sem.h"
#include"global.h"
#include"my_string.h"
#include"msg.h"

Flag getAnswer()
{
	struct MsgBufAnswer bufAnswer;
	int length = msgrcv(msgid, (struct msg_buf *)&bufAnswer, sizeof(struct DataForAnswer), getpid(), 0);
	CHECK("msgrcv", length);

	if(length < (int)sizeof(struct DataForAnswer))
	{
		printf("Too short message\n");
		return -1;
	}

	return bufAnswer.data.ACK;
}

Flag sendToHandler(String * recipient, String * data)
{
	sendToPipe(MSG, recipient, NULL, data);

	return getAnswer();
}

Flag createAccount(String * login, String * password)
{
	sendToPipe(REG, login, password, NULL);

	return getAnswer();
}

Flag checkAccount(String * login, String * password)
{
	sendToPipe(LOGIN, login, password, NULL);

	return getAnswer();
}

void sendToPipe(enum MessageType type, String * login, String * password, String * data)
{
	int result;

	semOperation(semid, fifoSynch, -1);

	switch(type)
	{
		case REG:
			result = write(fifo, "reg\n", 4);
			if(result != 4)
			{
				printf("write type error\n");
				exit(-1);
			}
			break;
		case LOGIN:
			result = write(fifo, "login\n", 6);
			if(result != 6)
			{
				printf("write type error\n");
				exit(-1);
			}

			break;
		case MSG:
			result = write(fifo, "msg\n", 4);
			if(result != 4)
			{
				printf("write type error\n");
				exit(-1);
			}

			break;
		case END:
			result = write(fifo, "end\n", 4);
			if(result != 4)
			{
				printf("write type error\n");
				exit(-1);
			}

			break;
		default:
			printf("sendToPipe type error\n");
			exit(-1);
	}

	result = printStringToStream(fifo, stringPid);
	CHECK("printStringToStream", result);
	write(fifo, "\n", 1);

	if(login)
	{
		result = printStringToStream(fifo, login);
		CHECK("printStringToStream", result);
		write(fifo, "\n", 1);
	}

	if(password)
	{
		result = printStringToStream(fifo, password);
		CHECK("printStringToStream", result);
		write(fifo, "\n", 1);
	}

	if(data)
	{
		result = printStringToStream(fifo, data);
		CHECK("printStringToStream", result);
	}

	semOperation(semid, fifoSynch, 1);
}
