#include"libs.h"
#include"global_variables.h"
#include"general_config.h"

#include"threads.h"
#include"general_config.h"
#include"sem_config.h"
#include"server_connection.h"
#include"server_api.h"

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
		CHECK("pthread_create", -1);

	semOperation(semid, startOfCommunication, 1);
}

void deleteThread()
{
	if(thid)
	{
		pthread_kill(thid, SIGUSR1);
		pthread_join(thid, (void **)NULL);
		thid = 0;
	}

	semOperation(semid, startOfCommunication, -1);
}
