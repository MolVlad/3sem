#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<fcntl.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>

#define PERMISSION 0777
#define BUF_SIZE 1500
#define STDOUT 1
#define ITERATION_NUM 10000000

#define CHECK(nameFunction, retValue)				\
do {								\
	if(retValue == -1)					\
	{							\
		printf("%s failure\n", nameFunction);		\
		exit(1);					\
	}							\
	else							\
		printf("%s succeeded\n", nameFunction);		\
} while(0)							\

void semAdd(int num, int semid)
{
	struct sembuf semBuf;

	semBuf.sem_op = num;
	semBuf.sem_flg = 0;
	semBuf.sem_num = 0;

	int result = semop(semid, &semBuf, 1);
	CHECK("semop", result);
	printf("sem_op = %d\n", num);
}

int main()
{
	const char file[] = "shm+sem.c";

	key_t key = ftok(file, 0);
	CHECK("ftok", key);
	printf("Key = %d\n", key);

	int semid = semget(key, 1, PERMISSION | IPC_CREAT | IPC_EXCL);
	if(semid < 0)
	{
		if(errno != EEXIST)
		{
			printf("Some error");
			return 4;
		}
		else
		{
			printf("Sem already exist\n");
			semid = semget(key, 1, PERMISSION);
			CHECK("semget", semid);
		}
	}
	else
	{
		printf("Sem create and get succesfully\n");
		semAdd(1, semid);
	}

	int shmid = shmget(key, sizeof(int), PERMISSION | IPC_CREAT | IPC_EXCL);
	if(shmid < 0)
	{
		if(errno != EEXIST)
		{
			printf("Some error");
			return 4;
		}
		else
		{
			printf("Memory already exist\n");
			shmid = shmget(key, sizeof(int), PERMISSION);
			CHECK("shmget", shmid);
		}
	}
	else
		printf("Memory get succesfully\n");

	int * pieceOfMemory = (int *)shmat(shmid, NULL, 0);
	if(pieceOfMemory == (int *)(-1))
	{
		printf("Attach memory error\n");
		return 5;
	}
	else
		printf("Attach memory succesfully\n");

	semAdd(-1, semid);

	*pieceOfMemory = 0;

	int i;
	for(i = 0; i < ITERATION_NUM; i++)
	{
		*pieceOfMemory += 1;
	}

	printf("%d\n", *pieceOfMemory);
	*pieceOfMemory = 0;

	semAdd(1, semid);

	CHECK("shmdt", shmdt(pieceOfMemory));

	return 0;
}

