#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>

#define BUF_SIZE 10

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		printf("Error arg\n");
		return 1;
	}

	char buf[BUF_SIZE];
	int n;

	mkfifo(argv[1], 0666);
	mkfifo(argv[2], 0666);

	int fifo[2];

	int pid = fork();

	if(pid == 0)
	{
		fifo[0] = open(argv[1], O_WRONLY);

		while(n)
		{
			n = read(1, buf, BUF_SIZE);
			write(fifo[0], buf, n);
		}

		close(fifo[0]);
	}
	else
	{
		fifo[1] = open(argv[2], O_RDONLY);

		while(n)
		{
			n = read(fifo[1], buf, BUF_SIZE);
			write(1, buf, n);
		}

		close(fifo[1]);
	}

	return 0;
}
