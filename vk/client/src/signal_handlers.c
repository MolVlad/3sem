#include"libs.h"
#include"global_variables.h"

#include"signal_handlers.h"
#include"server_connection.h"
#include"sem_config.h"

void configSignals()
{
	(void) signal(SIGPIPE, mainSigHandler);
	(void) signal(SIGUSR1, threadSidHandler);
	(void) signal(SIGINT, mainSigHandler);
}

void mainSigHandler()
{
	sendViaNet(END);
	remove(stringKey->data);
	deleteString(stringKey);
	close(sockfd);
	if(thid)
	{
		pthread_kill(thid, SIGUSR1);
		pthread_join(thid, (void **)NULL);
	}

	exit(-1);
}

void threadSidHandler()
{
	int status;
	semOperation(semid, startOfCommunication, 1);
	pthread_exit(&status);
}


