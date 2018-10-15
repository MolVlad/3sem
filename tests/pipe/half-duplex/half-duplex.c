#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>

#define _GNU_SOURCE 1
#include<fcntl.h>
#include<unistd.h>

#define BUF_SIZE 10
#define PERMISSION 0777
#define STDOUT 1

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

void semOperation(int num, int semid)
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
	const char pathname[] = "half-duplex.c";

	key_t key = ftok(pathname, 0);
	CHECK("ftok", key);
	printf("key = %d\n", key);

	int semid = semget(key, 1, PERMISSION | IPC_CREAT | IPC_EXCL);
	if(semid < 0)
	{
		if(errno == EEXIST)
		{
			printf("Already exist. Need to remove\n");

			semid = semget(key, 1, PERMISSION);
			CHECK("semget", semid);

			CHECK("semctl", semctl(semid, 0, IPC_RMID, 0));

			semid = semget(key, 1, PERMISSION | IPC_CREAT | IPC_EXCL);
			CHECK("semid", semid);
		}
		else
		{
			printf("Some error");
			return 4;
		}
	}
	else
		printf("Sem get succesfully\n");

	semOperation(1, semid);

	int fdPipe[2];
	int result = pipe2(fdPipe, O_NONBLOCK);
	CHECK("pipe2", result);

	int pid = fork();

	if(pid == 0)
	{
		semOperation(-1, semid);

		const char childFileName[] = "child.txt";

		int file = open(childFileName, O_RDONLY);
		CHECK("open", file);

		char buf[BUF_SIZE];
		int n;

		do {
			n = read(fdPipe[0], &buf, BUF_SIZE);
			write(STDOUT, &buf, n);
		} while(n);

		do {
			n = read(file, &buf, BUF_SIZE);
			write(fdPipe[1], &buf, n);
		} while(n);

		semOperation(1, semid);
	}
	else
	{
		semOperation(-1, semid);

		const char childFileName[] = "parent.txt";

		int file = open(childFileName, O_RDONLY);
		CHECK("open", file);

		char buf[BUF_SIZE];
		int n;

		do {
			n = read(fdPipe[0], &buf, BUF_SIZE);
			write(STDOUT, &buf, n);
		} while(n);

		do {
			n = read(file, &buf, BUF_SIZE);
			write(fdPipe[1], &buf, n);
		} while(n);

		semOperation(1, semid);
	}

	close(fdPipe[0]);
	close(fdPipe[1]);

	return 0;
}
