#include"libs.h"
#include"sending_instruction_by_handler.h"
#include"my_string.h"
#include"msg_queue_config.h"
#include"fifo_config.h"
#include"global.h"

void replyWithMSG(int pid, Flag isOK)
{
	struct MsgQueueBufAnswer msgQueueBufAnswer;
	msgQueueBufAnswer.type = pid;
	msgQueueBufAnswer.data.ACK = isOK;

	int result = msgsnd(msgidForAnswer, (struct msgbuf *)&msgQueueBufAnswer, sizeof(struct DataForAnswer), 0);
	CHECK("msgsnd", result);
}

void sendToRecipientPrivateFifoName(int pid)
{
	struct MsgQueueBufTransmission msgQueueBufTransmission;
	msgQueueBufTransmission.type = pid;

	strcpy(msgQueueBufTransmission.data.privateFifo, privateFifoName);

	int result = msgsnd(msgidForTransmission, (struct msgbuf *)&msgQueueBufTransmission, sizeof(struct DataForTransmission), 0);
	CHECK("msgsnd", result);
}

void sendToSenderPrivateFifoName(int pid, Flag isOK)
{
	struct MsgQueueBufAnswer msgQueueBufAnswer;
	msgQueueBufAnswer.type = pid;
	msgQueueBufAnswer.data.ACK = isOK;

	if(isOK == TRUE)
		strcpy(msgQueueBufAnswer.data.privateFifo, privateFifoName);

	int result = msgsnd(msgidForAnswer, (struct msgbuf *)&msgQueueBufAnswer, sizeof(struct DataForAnswer), 0);
	CHECK("msgsnd", result);
}

int scanPid()
{
	String * pid = createString();

	int result = scanStringFromStream(generalFifo, pid, -1);
	CHECK("scanStringFromStream", result);

	int ret = atoi(pid->data);

	deleteString(pid);

	return ret;
}

void increasePrivateFifoName(char * privateFifoName)
{
	int size = strlen(privateFifoName);
	int i;
	for(i = size - 1; i >=0; i--)
	{
		if(privateFifoName[i] < '9')
		{
			privateFifoName[i]++;
			return;
		}
		else
			privateFifoName[i] = '0';
	}

	memset(privateFifoName, '0', size);
}

