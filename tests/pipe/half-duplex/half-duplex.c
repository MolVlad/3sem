#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>

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
	int fd[2];
	char buf[BUF_SIZE];

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
		printf("Memory get succesfully\n");

	semAdd(1, semid);

	pipe(fd);

	int pid = fork();

	if(pid == 0)
	{
		semAdd(-1, semid);

		close(fd[1]);

		const char childFileName[] = "child.txt";

		int file = open(childFileName, O_RDONLY);
		CHECK("open", file);

		int n;

		do {
			n = read(file, &buf, BUF_SIZE);
			write(STDOUT, &buf, n);
		} while(n);

		close(fd[0]);

		semAdd(1, semid);

		exit(0);
	}
	else
	{
		semAdd(-1, semid);

		close(fd[0]);

		const char parentFileName[] = "parent.txt";

		int file = open(parentFileName, O_RDONLY);
		CHECK("open", file);

		int n;

		do {
			n = read(file, &buf, BUF_SIZE);
			write(STDOUT, &buf, n);
		} while(n);

		close(fd[0]);

		semAdd(1, semid);

		exit(0);
	}

	return 0;
}
