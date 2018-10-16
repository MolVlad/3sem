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
#define MESSAGE_FOR_PID 1
#define MESSAGE_FOR_END 2

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
	const char file[] = "child_msg.c";

	if(argc != 2)
	{
		printf("Arg error\n");
		return 8;
	}

	printf("\nLet's get started\n\n");

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
		int pid;
	} buf;

	int n = atoi(argv[1]);

	printf("\nLet's born\n");
	int i, pid, result, length;
	for(i = 0; i < n; i++)
	{
		pid = fork();
		if(pid == -1)
		{
			printf("fork failure\n");
			return 7;
		}
		else if(pid == 0)
		{
			printf("\nI was born!\n");
			int mypid = getpid();
			printf("My pid = %d\n", mypid);

			buf.type = MESSAGE_FOR_PID;
			buf.pid = mypid;
			result = msgsnd(msgid, (struct msgbuf *)&buf, sizeof(int), 0);
			CHECK("msgsnd", result);

			buf.type = MESSAGE_FOR_END;
			result = msgsnd(msgid, (struct msgbuf *)&buf, 0, 0);
			CHECK("msgsnd", result);

			exit(0);
		}
		else
		{
			length = msgrcv(msgid, (struct msg_buf *)&buf, 0, MESSAGE_FOR_END, 0);
			CHECK("msgrcv", length);
		}
	}

	printf("\nLet's get printed\n\n");
	while(1)
	{
		length = msgrcv(msgid, (struct msg_buf *)&buf, sizeof(int), MESSAGE_FOR_PID, IPC_NOWAIT);
		if((length == -1) && (errno != ENOMSG))
		{
			printf("msgrcv some error\n");
			return 6;
		}
		else if(length < sizeof(int))
		{
			printf("Too short message\n");
			return 6;
		}
		else if(errno == ENOMSG)
		{
			printf("That's all\n");
			CHECK("msgctl", msgctl(msgid, IPC_RMID, (struct msqid_ds *)NULL));
			return 0;
		}
		else
		{
			printf("msgrcv succeed\n");
			printf("pid = %d\n", buf.pid);
		}
	}
}
