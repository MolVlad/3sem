#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>

#define BUF_SIZE 10

int main()
{
	int fd[2];
	int fd_read = open("lol.txt", O_RDONLY);
	char buf[BUF_SIZE];

	pipe(fd);

	int pid = fork();

	if(pid == 0)
	{
		close(fd[1]);

		int n = 1;

		while(n)
		{
			n = read(fd[0], &buf, BUF_SIZE);
			write(1, &buf, n);
		}

		close(fd[0]);

		exit(0);
	}
	else
	{
		close(fd[0]);

		int n = 1;

		while(n)
		{
			n = read(fd_read, &buf, BUF_SIZE);
			write(fd[1], &buf, n);
		}

		close(fd[1]);

		exit(0);
	}

	return 0;
}
