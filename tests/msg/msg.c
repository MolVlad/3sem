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
#define BUF_SIZE 1500
#define STDOUT 1
#define MSG_SIZE 5
#define MESSAGE_FOR_TR 1
#define MESSAGE_FOR_RC 2

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
	if(argc != 2)
	{
		printf("Arg error\n");
		return 1;
	}

	const char file[] = "msg.c";

	if(atoi(argv[1]) == 1)	//transmitter
	{
		printf("Transmitter\n");

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

		struct msgBuf
		{
			long type;
			int num[MSG_SIZE];
		} buf;

		int i, num;
		for(i = 0; i < MSG_SIZE; i++)
		{
			printf("Scan number = ");
			scanf("%d", &buf.num[i]);
		}

		printf("Let's get transmitted\n");
		buf.type = MESSAGE_FOR_RC;
		int result = msgsnd(msgid, (struct msgbuf *)&buf, MSG_SIZE * sizeof(int), 0);
		CHECK("msgsnd", result);

		result = msgsnd(msgid, (struct msgbuf *)&buf, 0, 0);
		CHECK("msgsnd", result);
		printf("Transmition completed\n");

		printf("Let's get received\n");
		int length, sum = 0;
		do {
			length = msgrcv(msgid, (struct msg_buf *)&buf, sizeof(int), MESSAGE_FOR_TR, 0);
			CHECK("msgrcv", length);

			if(length)
				printf("Sum = %d\n", buf.num[0]);
		} while(length);
		printf("Reception completed\n");

		CHECK("msgctl", msgctl(msgid, IPC_RMID, (struct msqid_ds *)NULL));
	}
	else if(atoi(argv[1]) == 0)	//reciever
	{
		printf("Receiver\n");

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

		struct msgBuf
		{
			long type;
			int num[MSG_SIZE];
		} buf;

		printf("Let's get received\n");
		int length, i, sum = 0;
		do {
			length = msgrcv(msgid, (struct msg_buf *)&buf, MSG_SIZE * sizeof(int), 0, 0);
			CHECK("msgrcv", length);

			if(length)
			{
				for(i = 0; i < MSG_SIZE; i++)
					sum += buf.num[i];
				printf("sum = %d\n", sum);
			}
		} while(length);
		printf("Reception completed\n");

		printf("Let's get transmitted\n");
		buf.type = MESSAGE_FOR_TR;
		buf.num[0] = sum;
		int result = msgsnd(msgid, (struct msgbuf *)&buf, sizeof(int), 0);
		CHECK("msgsnd", result);

		result = msgsnd(msgid, (struct msgbuf *)&buf, 0, 0);
		CHECK("msgsnd", result);
		printf("Transmition completed\n");
	}
	else
	{
		printf("Arg error\n");
		return 1;
	}

	return 0;
}




