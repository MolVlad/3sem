#include "main.h"

/* Main function -----------------------------------------------------------------*/

int main()
{
	splitStruct split;
	split.maxSizeText = 		MAX_SIZE_TEXT;
	split.maxSizeDelimiters = 	MAX_SIZE_DELIMITERS;
	split.maxNumWords = 		MAX_NUM_WORDS;
	split.maxSizeWord = 		MAX_SIZE_WORD;

	if(splitInit(&split))
	{
		printf("Init error\n");
		return 1;
	}

	#ifndef SCAN_DELIMITERS
	char delimiters[] = " \n\t";
	strcpy(split.delimiters, delimiters);
	#endif /* SCAN_DELIMITERS */

	int thereIsCommand = 1;

	while(thereIsCommand)
	{
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

		

		//createTask();

		if(splitPrint(&split))
		{
			printf("Print error\n");
			return 4;
		}

		thereIsCommand = 0;
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
