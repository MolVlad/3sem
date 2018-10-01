#include "main.h"

/* Main function -----------------------------------------------------------------*/

int main()
{
	splitStruct split;
	split.maxSizeText = 		MAX_SIZE_TEXT;
	split.maxSizeDelimiters = 	MAX_SIZE_DELIMITERS;
	split.maxNumWords = 		MAX_NUM_WORDS;
	split.maxSizeWord = 		MAX_SIZE_WORD;

	int i, numberOfTask;
	scanf("%d", &numberOfTask);
	getchar();

	int status;
	int pid;

	for(i = 0; i < numberOfTask; i++)
	{
		if(splitInit(&split))
		{
			printf("Init error\n");
			return 1;
		}

		char delimiters[] = " \n\t";
		strcpy(split.delimiters, delimiters);

		if(splitScan(&split))
		{
			printf("Scan error\n");
			return 2;
		}

		if(splitSplit(&split))
		{
			printf("Split error\n");
			return 3;
		}

		split.words[split.count + 1] = NULL;

		pid = fork();

		if(pid == 0)
		{
			pid = fork();

			if(pid == 0)
			{
				sleep(split.delay);
				execvp(split.words[0], &split.words[2]);
			}
			else
			{
				sleep(split.delay + MAX_TIME);

				if(waitpid(pid, &status, WNOHANG))
				{
					printf("child has done\n");
					exit(0);
				}
				else
				{
					printf("child hasn't done\n");
					//kill(pid, )****************
					exit(0);
				}
			}
		}
	}

	if(splitFree(&split))
	{
		printf("Free error\n");
		return 5;
	}

	return 0;
}

/*int main(int argc, char * argv[], char *envp[])
{
	int status;
	int pid = fork();

	if(pid == 0)
	{
		pid = fork();
		if(pid == 0)
			execlp("gcc", "gcc", "hello.c", "-o", "prog", NULL);
		else
		{
			wait(&status);
			execlp("./prog", "./prog", NULL);
		}
	}
	else
	{
		wait(&status);
		printf("that's success!\n");
	}

	return 0;
}*/
