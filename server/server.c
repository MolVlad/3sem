#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<fcntl.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/sem.h>

#define PERMISSION 0777
#define MSG_SIZE 2
#define MESSAGE_FOR_SERVER 1
#define NUM 1

#define CHECK(nameFunction, retValue)				\
do {								\
	if(retValue == -1)					\
	{							\
		perror("nameFunction");				\
		exit(1);					\
	}							\
} while(0)							\

void *processRequest(void * arg);
int connectToSem(key_t key, int num);
int createSem(key_t key, int num);
void semOperation(int semid, int num, int operation);

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

typedef struct Segment
{
	int msgid;
	key_t key;
	struct DataForRequest data;
} Segment;

void copyData(struct DataForRequest * destination, struct DataForRequest * data)
{
	destination->pid = data->pid;

	int i;
	for(i = 0; i < MSG_SIZE; i++)
		destination->num[i] = data->num[i];
}

void *processRequest(void * arg)
{
	Segment * segment = (Segment *)arg;

	printf("Message from %d. Wait\n", segment->data.pid);
	int semid = connectToSem(segment->key, 1);
	semOperation(semid, 0, -1);
	printf("Have waited!\n");
	int answer = segment->data.num[0] * segment->data.num[1];

	struct MsgBufAnswer bufAnswer;
	bufAnswer.type = segment->data.pid;
	bufAnswer.data.answer = answer;

	//printf("Accepted %d and %d\n", segment->data.num[0], segment->data.num[1]);
	//printf("Res = %d\n", answer);

	printf("wait 5 sec\n");
	sleep(5);
	int result = msgsnd(segment->msgid, (struct msgbuf *)&bufAnswer, sizeof(struct DataForAnswer), 0);
	CHECK("msgsnd", result);

	printf("Done\n");
	semOperation(semid, 0, 1);

	free(segment);
	pthread_exit(0);
}

int createSem(key_t key, int num)
{
	int semid = semget(key, num, PERMISSION | IPC_CREAT | IPC_EXCL);
	if(semid < 0)
	{
		if(errno == EEXIST)
		{
			printf("Sem already exist. Need to remove\n");

			semid = semget(key, num, PERMISSION);
			CHECK("semget", semid);

			CHECK("semctl", semctl(semid, 0, IPC_RMID, 0));

			semid = semget(key, num, PERMISSION | IPC_CREAT | IPC_EXCL);
			CHECK("semid", semid);
		}
		else
		{
			perror("semget");
			return 2;
		}
	}

	return semid;
}

int connectToSem(key_t key, int num)
{
	int semid;
	do semid = semget(key, num, PERMISSION);
	while((semid == -1) && (errno ==ENOENT));

	if(semid == -1)
	{
		perror("semget");
		exit(4);
	}

	return semid;
}

void semOperation(int semid, int num, int operation)
{
	struct sembuf semBuf;

	semBuf.sem_op = operation;
	semBuf.sem_flg = 0;
	semBuf.sem_num = num;

	int result = semop(semid, &semBuf, 1);
	CHECK("semop", result);
}

int main(int argc, char *argv[])
{
	const char file[] = "server.c";

	if(argc == 2)	//server
	{
		printf("Server\n");

		key_t key = ftok(file, 0);
		CHECK("ftok", key);

		int semid = createSem(key, 1);
		semOperation(semid, 0, NUM);

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
				perror("msgget");
				return 4;
			}
		}
		printf("Let's get started processing requests\n");

		struct MsgBufRequest bufRequest;
		struct MsgBufAnswer bufAnswer;

		int length, result;
		pthread_t thid;
		while(1)
		{
			length = msgrcv(msgid, (struct msg_buf *)&bufRequest, sizeof(struct DataForRequest), MESSAGE_FOR_SERVER, 0);
			if(length)
			{
				struct Segment * segment = (struct Segment *)calloc(1, sizeof(struct Segment));
				copyData(&segment->data, &bufRequest.data);
				segment->msgid = msgid;
				segment->key = key;

				if(length < sizeof(struct DataForRequest))
					printf("Too short message\n");
				else
				{
					result = pthread_create(&thid, NULL, processRequest, segment);
					if(result)
					{
						printf("pthread_create error\n");
						return -1;
					}
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
		CHECK("msgget", msgid);

		struct MsgBufRequest bufRequest;
		struct MsgBufAnswer bufAnswer;

		int a,b;
		printf("Scan a = ");
		scanf("%d", &a);
		printf("Scan b = ");
		scanf("%d", &b);

		int pid = getpid();

		printf("Let's get transmitted\n");

		bufRequest.type = MESSAGE_FOR_SERVER;
		bufRequest.data.pid = pid;
		bufRequest.data.num[0] = a;
		bufRequest.data.num[1] = b;

		int result = msgsnd(msgid, (struct msgbuf *)&bufRequest, sizeof(struct DataForRequest), 0);
		CHECK("msgsnd", result);

		printf("Let's get received\n");
		int length = msgrcv(msgid, (struct msg_buf *)&bufAnswer, sizeof(struct DataForAnswer), pid, 0);
		CHECK("msgrcv", length);

		if(length < sizeof(struct DataForAnswer))
		{
			printf("Too short message\n");
			return 2;
		}
		else
			printf("Answer = %d\n", bufAnswer.data.answer);
	}
	else
	{
		printf("Arg error\n");
		return 1;
	}

	return 0;
}
