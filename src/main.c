#include "precomp.h"
#include "split.h"

/* Main function -----------------------------------------------------------------*/

int main()
{
	// Мне не нравится название splitStruct
	// 1) Пишите первую букву названия заглавной
	// 2) Слово Struct не несет никакой смысловой нагрузки. Это скорее некое описание команды для запуска по своей сути, а не абстрактный набор текста и слов.
	splitStruct split;
	split.maxSizeText = 		MAX_SIZE_TEXT;
	split.maxSizeDelimiters = 	MAX_SIZE_DELIMITERS;
	split.maxNumWords = 		MAX_NUM_WORDS;
	split.maxSizeWord = 		MAX_SIZE_WORD;

	int i, numberOfTask;
	scanf("%d\n", &numberOfTask);

	int status;

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

		// Смотря только в код сложно понять, зачем вам нужно создавать два процесса.
		// Я бы создал хотя бы две отдельные переменные для pid'а процесса таймера и того, где запуск команды происходит
		int pid = fork();

		if(pid == 0)
		{
			int new_pid = fork();

			if(new_pid == 0)
			{
				sleep(split.delay);
				execvp(split.words[0], &split.words[2]);
			}
			else
			{
				sleep(split.delay + MAX_TIME);

				if(waitpid(pid, &status, WNOHANG))
					exit(0);
				else
				{
					kill(pid, SIGKILL);
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
