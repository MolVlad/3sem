#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

#define BUF_SIZE 100
#define STDOUT 1
#define STDIN 0

int main()
{
	char * cat_args[] = {"cat", "lol.txt", NULL};
	char * grep_args[] = {"grep", "az", NULL};

	int fdPipe[2];
	pipe(fdPipe);

	int pid = fork();

	if(pid == 0)
	{
		close(fdPipe[0]);
		dup2(fdPipe[1], STDOUT);
		execvp("cat", cat_args);
	}
	else
	{
		close(fdPipe[1]);
		dup2(fdPipe[0], STDIN);
		execvp("grep", grep_args);
	}

	return 0;
}
