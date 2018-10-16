#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<fcntl.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>

#define PERMISSION 0777
#define MSG_SIZE 2
#define MESSAGE_FOR_SERVER 1

#define CHECK(nameFunction, retValue)				\
do {								\
	if(retValue == -1)					\
	{							\
		printf("%s failure\n", nameFunction);		\
		return 1;					\
	}							\
	else							\
		printf("%s succeeded\n", nameFunction);		\
} while(0)							\

int main(int argc, char *argv[])
{
	const char file[] = "server.c";

	if(argc == 2)	//server
	{
		printf("Server\n");

		key_t key = ftok(file, 0);
		CHECK("ftok", key);
		printf("Key = %d\n", key);

		int msgid = msgget(key, PERMISSION | IPC_CREAT | IPC_EXCL);
		if(msgid < 0)
		{
			if(errno == EEXIST)
			{
				printf("Already exist. Need to remove\n");

				msgid = msgget(key, PERMISSION);
				CHECK("msgget", msgid);

				CHECK("msgctl", msgctl(msgid, IPC_RMID, (struct msqid_ds *)NULL));

				msgid = msgget(key, PERMISSION | IPC_CREAT | IPC_EXCL);
				CHECK("msgid", msgid);
			}
			else
			{
				printf("Some error");
				return 4;
			}
		}
		else
			printf("Message queue get succesfully\n");

		struct msgBufRequest
		{
			long type;
			struct Data
			{
				int pid;
				int num[MSG_SIZE];
			} data;
		} bufRequest;

		struct msgBufAnswer
		{
			long type;
			int answer;
		} bufAnswer;

		printf("Let's get started processing requests\n");
		int length, answer, result;
		while(1)
		{
			length = msgrcv(msgid, (struct msg_buf *)&bufRequest, sizeof(struct Data), -(MESSAGE_FOR_SERVER + 1), 0);
			if(length)
			{
				printf("Message from %d\n", bufRequest.data.pid);

				if(length < sizeof(struct Data))
					printf("Too short message\n");
				else
				{
					answer = bufRequest.data.num[0] + bufRequest.data.num[1];

					bufAnswer.type = bufRequest.data.pid;
					bufAnswer.answer = answer;

					printf("Accepted %d and %d\n", bufRequest.data.num[0], bufRequest.data.num[1]);
					printf("Sum = %d\n", answer);

					result = msgsnd(msgid, (struct msgbuf *)&bufAnswer, sizeof(int), 0);
					CHECK("msgsnd", result);
				}
			}
		}
	}
	else if(argc == 1)	//client
	{
		printf("Client\n");

		key_t key = ftok(file, 0);
		CHECK("ftok", key);

		printf("Key = %d\n", key);

		int msgid;
		do msgid = msgget(key, PERMISSION);
		while((msgid == -1) && (errno ==ENOENT));

		if(msgid == -1)
		{
			printf("Some error");
			return 4;
		}
		printf("msgget succeed\n");

		struct msgBufRequest
		{
			long type;
			struct Data
			{
				int pid;
				int num[MSG_SIZE];
			} data;
		} bufRequest;

		struct msgBufAnswer
		{
			long type;
			int answer;
		} bufAnswer;

		int a,b;
		printf("Scan a = ");
		scanf("%d", &a);
		printf("Scan b = ");
		scanf("%d", &b);

//		a = 5, b = 5;

		int pid = getpid();

		printf("Let's get transmitted\n");
		bufRequest.type = MESSAGE_FOR_SERVER;
		bufRequest.data.pid = pid;
		bufRequest.data.num[0] = a;
		bufRequest.data.num[1] = b;
		int result = msgsnd(msgid, (struct msgbuf *)&bufRequest, sizeof(struct Data), 0);
		CHECK("msgsnd", result);

		printf("Let's get received\n");
		int length, i, answer;
		length = msgrcv(msgid, (struct msg_buf *)&bufAnswer, sizeof(int), pid, 0);
		CHECK("msgrcv", length);

		if(length < sizeof(int))
		{
			printf("Too short message\n");
			return 2;
		}
		else
			printf("Answer = %d\n", bufAnswer.answer);
	}
	else
	{
		printf("Arg error\n");
		return 1;
	}

	return 0;
}




