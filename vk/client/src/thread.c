#include"libs.h"
#include"global.h"
#include"thread.h"
#include"general_config.h"
#include"sem.h"
#include"menu.h"

void sigHandler()
{
	sendViaNet(END);
	remove(stringKey->data);
	deleteString(stringKey);

	exit(-1);
}

void threadSidHandler()
{
	int status;
	semOperation(semid, startOfCommunication, 1);
	pthread_exit(&status);
}

void *thread()
{
	semOperation(semid, startOfCommunication, -1);

	while(1)
	{
		semOperation(semid, communicationWithServer, -1);

		Flag isAll;
		do
		{
			isAll = receiveMessage();
		} while(isAll == TRUE);

		semOperation(semid, communicationWithServer, 1);

		sleep(1);
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

	semOperation(semid, startOfCommunication, 1);
}

void deleteThread()
{
	if(thid)
	{
		pthread_kill(thid, SIGUSR1);
		thid = 0;
	}

	semOperation(semid, startOfCommunication, -1);
}
