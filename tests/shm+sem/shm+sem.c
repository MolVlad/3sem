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
#define NUM_CYCLE 50

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

int main()
{
	const char file[] = "shm.c";

	int fd = open(file, O_RDONLY);
	CHECK("open", fd);

	key_t key = ftok(file, 0);
	CHECK("ftok", key);
	printf("Key = %d\n", key);

	int shmid = shmget(key, BUF_SIZE * sizeof(char), PERMISSION | IPC_CREAT | IPC_EXCL);
	if(shmid < 0)
	{
		if(errno == EEXIST)
		{
		printf("Already exist. Need to remove\n");

			shmid = shmget(key, BUF_SIZE * sizeof(char), PERMISSION);
			CHECK("shmget", shmid);

			CHECK("shmctl", shmctl(shmid, IPC_RMID, NULL));

			shmid = shmget(key, BUF_SIZE * sizeof(char), PERMISSION | IPC_CREAT | IPC_EXCL);
			CHECK("shmid", shmid);
		}
		else
		{
			printf("Some error");
			return 4;
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

	int i;
	for(i = 0; i < NUM_CYCLE; i++)
	{
		*pieceOfMemory += 1;
		printf("pid = %d, pieceOfMemory = %d\n", getpid(), *pieceOfMemory);
	}

	CHECK("shmdt", shmdt(pieceOfMemory));

	return 0;
}

