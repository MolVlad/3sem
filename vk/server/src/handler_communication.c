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
#include"fifo.h"

Flag getAnswer()
{
	struct MsgBufAnswer bufAnswer;
	int length = msgrcv(msgidForAnswer, (struct msg_buf *)&bufAnswer, sizeof(struct DataForAnswer), getpid(), 0);
	CHECK("msgrcv", length);

	if(length < (int)sizeof(struct DataForAnswer))
	{
		printf("Too short message\n");
		return -1;
	}

	return bufAnswer.data.ACK;
}

Flag getPrivateFifoToWrite()
{
	struct MsgBufAnswer bufAnswer;
	int length = msgrcv(msgidForAnswer, (struct msg_buf *)&bufAnswer, sizeof(struct DataForAnswer), getpid(), 0);
	CHECK("msgrcv", length);

	if(length < (int)sizeof(struct DataForAnswer))
	{
		CHECK("msgrcv short msg", -1);
	}

	if(bufAnswer.data.ACK == TRUE)
		strcpy(privateFifoName, bufAnswer.data.privateFifo);

	return bufAnswer.data.ACK;
}

Flag getPrivateFifoToRead()
{
	struct MsgBufTransmission bufTransmission;
	int length = msgrcv(msgidForTransmission, (struct msg_buf *)&bufTransmission,
			sizeof(struct DataForTransmission), getpid(), IPC_NOWAIT);
	if((length == -1) && (errno = ENOMSG))
		return FALSE;
	CHECK("msgrcv", length);

	if(length < (int)sizeof(struct DataForTransmission))
	{
		CHECK("msgrcv short msg", -1);
	}

	strcpy(privateFifoName, bufTransmission.data.privateFifo);
	return TRUE;
}

Flag sendToRecipient(String * recipient, String * data)
{
	sendToPipe(MSG, recipient, NULL);

	Flag isOK = getPrivateFifoToWrite();
	if(isOK == TRUE)
	{
		int privateFifoToWrite = openFIFO(privateFifoName);
		printStringToStream(privateFifoToWrite, recipient);
		write(privateFifoToWrite, "\n", 1);
		printStringToStream(privateFifoToWrite, data);
		close(privateFifoToWrite);
	}

	return isOK;
}

Flag createAccount(String * login, String * password)
{
	sendToPipe(REG, login, password);

	return getAnswer();
}

Flag checkAccount(String * login, String * password)
{
	sendToPipe(LOGIN, login, password);

	return getAnswer();
}

void sendToPipe(enum MessageType type, String * login, String * password)
{
	int result;
	Flag error = FALSE;

	semOperation(semid, generalFifoSynch, -1);

	switch(type)
	{
		case REG:
			result = write(generalFifo, "reg\n", 4);
			if(result != 4)
				error = TRUE;
			break;
		case LOGIN:
			result = write(generalFifo, "login\n", 6);
			if(result != 6)
				error = TRUE;

			break;
		case MSG:
			result = write(generalFifo, "msg\n", 4);
			if(result != 4)
				error = TRUE;

			break;
		case END:
			result = write(generalFifo, "end\n", 4);
			if(result != 4)
				error = TRUE;

			break;
		default:
			error = TRUE;
	}

	if(error == TRUE)
	{
		printf("sendToPipe type error\n");
		CHECK("write", -1);
	}

	result = printStringToStream(generalFifo, stringPid);
	CHECK("printStringToStream", result);
	write(generalFifo, "\n", 1);

	if(login)
	{
		result = printStringToStream(generalFifo, login);
		CHECK("printStringToStream", result);
		write(generalFifo, "\n", 1);
	}

	if(password)
	{
		result = printStringToStream(generalFifo, password);
		CHECK("printStringToStream", result);
		write(generalFifo, "\n", 1);
	}

	semOperation(semid, generalFifoSynch, 1);
}
