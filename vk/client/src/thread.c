#include"libs.h"
#include"global.h"
#include"thread.h"
#include"general_config.h"
#include"sem.h"
#include"menu.h"

void sigHandler(int nsig)
{
	printf("Print to stream error with SIGPIPE. nsig = %d\n", nsig);
	exit(-1);
}

void threadSidHandler(int nsig)
{
	printf("Print to stream error with SIGINT. nsig = %d\n", nsig);
	int status;
	pthread_exit(&status);
}

void *thread()
{
	printf("Thread for receive message was born\n");
	while(1)
	{
		semOperation(semid, startOfCommunication, -1);
		semOperation(semid, communicationWithServer, -1);

		printf("thread\n");

		Flag isAll;
		do
		{
			isAll = receiveMessage();
		} while(isAll == TRUE);

		semOperation(semid, communicationWithServer, 1);
		semOperation(semid, startOfCommunication, 1);
	}
}

void createThread()
{
	int  result = pthread_create(&thid, NULL, thread, NULL);
	if(result != 0)
	{
		perror("pthread create");
		exit(-1);
	}
}

void deleteThread()
{
	pthread_kill(thid, SIGINT);
	printf("delete thread\n");
}
