#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>

#define BUF_SIZE 10
#define NAME_SIZE 8
#define PERMISSION 0777

char * getname();
char* itoa(int val, int base);

int main(int argc, char *argv[])
{
	const char generalFIFO[] = "users";

	if(argc == 1)	//receive
	{
		int users = open(generalFIFO, O_RDWR);

		if(users == -1)
		{
			printf("Need to create %s\n", generalFIFO);
			mkfifo(generalFIFO, PERMISSION);
			users = open(generalFIFO, O_RDWR);
		}

		char name[NAME_SIZE];

		int n = 0;

		n = read(users, name, NAME_SIZE);
		if(n != NAME_SIZE)
		{
			printf("error read name\n");
			return 1;
		}

/*		while(strcmp(name, argv[1]))
		{
			n = write(users, &name, NAME_SIZE);
			if(n != NAME_SIZE)
			{
				printf("error write name\n");
				return 3;
			}

			n = read(users, &name, NAME_SIZE);
			if(n != NAME_SIZE)
			{
				printf("error read name\n");
				return 1;
			}
		}
*/
		mkfifo(name, PERMISSION);
		if(errno == EEXIST)
			printf("%s already exists\n", name);
		int receiveFIFO = open(name, O_RDONLY);

		printf("Start communication with %s\n", name);

		char buf[BUF_SIZE];

		do
		{
			n = read(receiveFIFO, &buf, BUF_SIZE);
			write(1, &buf, n);
		}
		while(n);

	}
	else if(argc == 2)	//transmit
	{
		char * name = getname();

		int users = open(generalFIFO, O_WRONLY);

		if(users == -1)
		{
			printf("Need to create %s\n", generalFIFO);
			mkfifo(generalFIFO, PERMISSION);
			users = open(generalFIFO, O_WRONLY);
		}

		int n = 0;

		n = write(users, name, NAME_SIZE);
		if(n != NAME_SIZE)
		{
			printf("error write name\n");
			return 3;
		}

		int transmitFIFO = open(name, O_WRONLY);
		printf("There is recipient! Let's get started!\n");

		char buf[BUF_SIZE];

		int file = open(argv[1], O_RDONLY);
		if(file < 0)
		{
			printf("File error\n");
			return 0;
		}

		printf("File open. Let's transmit!\n");

		do
		{
			n = read(file, &buf, BUF_SIZE);
			write(transmitFIFO, &buf, n);
		}
		while(n);

		if(read(file, &buf, 1))
			printf("An error was occured!\n");
		else
			printf("File transmitted succesfully!\n");
	}
	else
	{
		printf("Error arg\n");
		return 1;
	}

	return 0;
}

char * getname()
{
	char * pid = itoa(getpid(), 10);
	static char name[NAME_SIZE];

	strcpy(name, pid);

	int len = strlen(name);
	int i;

	for(i = len; i < NAME_SIZE; i++)
		name[i] = '_';

	return name;
}

char* itoa(int val, int base)
{
	static char buf[32] = {0};
	int i = 30;

	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];

	return &buf[i+1];
}
