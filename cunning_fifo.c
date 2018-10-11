#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include <errno.h>

#define BUF_SIZE 10
#define NAME_SIZE 4
#define PERMISSION 0777

int main(int argc, char *argv[])
{
	const char generalFIFO[] = "users";

	if(argc == 1)	//receive
	{
		int users = open(generalFIFO, O_RDONLY);

		if(users == -1)
		{
			printf("Need to create %s\n", generalFIFO);
			mkfifo(generalFIFO, PERMISSION);
			users = open(generalFIFO, O_RDONLY);
		}

		char name[NAME_SIZE];

		int n = 0;

		n = read(users, name, NAME_SIZE);
		if(n != NAME_SIZE)
		{
			printf("error read name\n");
			return 1;
		}

		mkfifo(name, PERMISSION);
		if(errno == EEXIST)
			printf("%s already exists\n", name);
		int receiveFIFO = open(name, O_RDONLY);

		printf("Start communication with %s\n", name);

		char buf[BUF_SIZE];

		while(1)
		{
			n = read(receiveFIFO, &buf, BUF_SIZE);
			write(1, &buf, n);
		}
	}
	else if(argc == 2)	//transmit
	{
		if(strlen(argv[1]) != NAME_SIZE)
		{
			printf("Name error: the name must contain 4 char\n");
			return 2;
		}

		int users = open(generalFIFO, O_WRONLY);

		if(users == -1)
		{
			printf("Need to create %s\n", generalFIFO);
			mkfifo(generalFIFO, PERMISSION);
			users = open(generalFIFO, O_WRONLY);
		}

		int n = 0;

		n = write(users, argv[1], NAME_SIZE);
		if(n != NAME_SIZE)
		{
			printf("error write name\n");
			return 3;
		}

		int transmitFIFO = open(argv[1], O_WRONLY);
		printf("There is recipient! Let's get started!\n");

		while(1)
		{
			static char buf[BUF_SIZE];
			n = read(1, &buf, BUF_SIZE);
			write(transmitFIFO, &buf, n);
		}
	}
	else
	{
		printf("Error arg\n");
		return 1;
	}

	return 0;
}
