#include"libs.h"
#include"signal_handlers.h"
#include"global_variables.h"

void configSignals()
{
	(void) signal(SIGINT, sigHandler);
	(void) signal(SIGUSR1, threadSidHandler);
}

void threadSidHandler()
{
	int status;
	pthread_exit(&status);
}

void sigHandler(int nsig)
{
	printf("Exit from server with signal SIGINT, nsig = %d\n", nsig);
	kill(handlerPid, SIGINT);
	close(sockfd);
	if(thid)
	{
		pthread_kill(thid, SIGUSR1);
		pthread_join(thid, (void **)NULL);
	}

	deleteString(stringPid);
	deleteString(userLogin);

	exit(0);
}
