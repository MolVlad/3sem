#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>

#define BUF_SIZE 10
#define NAME_SIZE 4

int main(int argc, char *argv[])
{
	const char generalFIFO[] = "users";

	if(argc == 1)	//receive
	{
		int users = open(generalFIFO, O_RDONLY);

		if(users == -1)
		{
			printf("need to create\n");
			mkfifo(generalFIFO, 0777);
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

		mkfifo(name, 0777);
		int receiveFIFO = open(name, O_RDONLY);

		while(1)
		{
			static char buf[BUF_SIZE];
			n = read(receiveFIFO, &buf, BUF_SIZE);
			write(1, &buf, n);
		}
	}
	else if(argc == 2)	//transmit
	{
		if(strlen(argv[1]) != NAME_SIZE)
		{
			printf("name error\n");
			return 2;
		}

		int users = open(generalFIFO, O_WRONLY);

		if(users == -1)
		{
			printf("need to create\n");
			mkfifo(generalFIFO, 0777);
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

		while(transmitFIFO == -1)
			transmitFIFO = open(argv[1], O_WRONLY);

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
