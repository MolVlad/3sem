#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<fcntl.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>

#define PERMISSION 0777
#define BUF_SIZE 1500
#define STDOUT 1

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
	const char file[] = "shm.c";

	if(atoi(argv[1]) == 1)	//receiver
	{
		printf("Receiver\n");

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

		char * pieceOfMemory = (char *)shmat(shmid, NULL, 0);
		if(pieceOfMemory == (char *) (-1))
		{
			printf("Attach memory error\n");
			return 5;
		}
		else
			printf("Attach memory succesfully\n");

		int n = read(fd, pieceOfMemory, BUF_SIZE);
		printf("Read %d chars from file\n", n);

		if(n == 0)
		{
			printf("Read error\n");
			return 6;
		}

		CHECK("shmdt", shmdt(pieceOfMemory));
	}
	else if(atoi(argv[1]) == 0)	//transmitter
	{
		printf("Transmitter\n");

		key_t key = ftok(file, 0);
		CHECK("ftok", key);

		printf("Key = %d\n", key);

		int shmid = shmget(key, BUF_SIZE * sizeof(char), 0);
		CHECK("shmid", shmid);

		char * pieceOfMemory = (char *)shmat(shmid, NULL, 0);
		if(pieceOfMemory == (char *) (-1))
		{
			printf("Attach memory error\n");
			return 5;
		}
		else
			printf("Attach memory succesfully\n");

		printf("%s\n", pieceOfMemory);

		CHECK("shmctl", shmctl(shmid, IPC_RMID, NULL));
		CHECK("shmdt", shmdt(pieceOfMemory));
	}
	else
	{
		printf("Arg error\n");
		return 1;
	}

	return 0;
}

