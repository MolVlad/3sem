#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>

#define PERMISSION 0777

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

int main(int argc, char * argv[])
{
	const char pathname[] = "sem_test.c";

	key_t key = ftok(pathname, 0);
	CHECK("ftok", key);
	printf("key = %d\n", key);

	int semid = semget(key, 1, PERMISSION | IPC_CREAT);
	CHECK("semget",semid);

	if(argc != 2)
	{
		printf("Arg error\n");
		return 1;
	}

	if(atoi(argv[1]) == 1)
	{
		printf("Programm sem D(S, 1)\n");

		struct sembuf buf;

		buf.sem_op = -5;
		buf.sem_flg = 0;
		buf.sem_num = 0;

		int result = semop(semid, &buf, 1);
		CHECK("semop", result);
	}
	else
	{
		printf("Programm sem A(S, 1)\n");

		struct sembuf buf;

		buf.sem_op = 1;
		buf.sem_flg = 0;
		buf.sem_num = 0;

		int result = semop(semid, &buf, 1);
		CHECK("semop", result);
	}

	return 0;
}
