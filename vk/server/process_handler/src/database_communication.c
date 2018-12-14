#include"libs.h"
#include"general_config.h"
#include"my_string.h"
#include"database_communication.h"
#include"sem_config.h"
#include"global_variables.h"
#include"my_string.h"
#include"msg_queue_config.h"
#include"fifo_config.h"

Flag getAnswer()
{
	struct MsgQueueBufAnswer msgQueueBufAnswer;
	int length = msgrcv(msgidForAnswer, (struct msg_buf *)&msgQueueBufAnswer, sizeof(struct DataForAnswer), getpid(), 0);
	CHECK("msgrcv", length);

	if(length < (int)sizeof(struct DataForAnswer))
	{
		printf("Too short message\n");
		return -1;
	}

	return msgQueueBufAnswer.data.ACK;
}

Flag getPrivateFifoToWrite()
{
	struct MsgQueueBufAnswer msgQueueBufAnswer;
	int length = msgrcv(msgidForAnswer, (struct msg_buf *)&msgQueueBufAnswer, sizeof(struct DataForAnswer), getpid(), 0);
	CHECK("msgrcv", length);

	if(length < (int)sizeof(struct DataForAnswer))
	{
		CHECK("msgrcv short msg", -1);
	}

	if(msgQueueBufAnswer.data.ACK == TRUE)
		strcpy(privateFifoName, msgQueueBufAnswer.data.privateFifo);

	return msgQueueBufAnswer.data.ACK;
}

Flag getPrivateFifoToRead()
{
	struct MsgQueueBufTransmission msgQueueBufTransmission;
	int length = msgrcv(msgidForTransmission, (struct msg_buf *)&msgQueueBufTransmission,
			sizeof(struct DataForTransmission), getpid(), IPC_NOWAIT);
	if((length == -1) && (errno = ENOMSG))
		return FALSE;
	CHECK("msgrcv", length);

	if(length < (int)sizeof(struct DataForTransmission))
	{
		CHECK("msgrcv short msg", -1);
	}

	strcpy(privateFifoName, msgQueueBufTransmission.data.privateFifo);
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
