#include"libs.h"
#include"general_config.h"
#include"my_string.h"
#include"handler_communication.h"
#include"user_communication.h"
#include"zombie.h"
#include"state.h"
#include"fifo.h"
#include"msg.h"
#include"sem.h"

pthread_t thid = 0;
int handlerPid;
int generalFifo;
int semid;
int msgidForAnswer;
int msgidForTransmission;
int sockfd;
String * stringPid;
String * userLogin;
char privateFifoName[PRIVATE_FIFO_NAME_SIZE];

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

int main()
{
	(void) signal(SIGINT, sigHandler);
	(void) signal(SIGUSR1, threadSidHandler);

	sockfd = raiseServer();

	int result = createThreadToFightZombie();
	CHECK("createThreadToFightZombie", result);

	handlerPid = fork();
	CHECK("fork", handlerPid);
	if(handlerPid == 0)
		execlp("handler/handler", "handler", NULL);

	while(1)
	{
		struct sockaddr_in cliaddr;
		unsigned int clilen = sizeof(cliaddr);
		int newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen);
		CHECK("accept", newsockfd);

		//communicate with user in other process
		if(fork() == 0)
		{
			//close first socket in child process
			close(sockfd);

			printf("New user. Pid for handle: %d\n", getpid());

			stringPid = pidToString();

			generalFifo = openFIFO(GENERAL_FIFO);

			key_t firstKey = getTheKey(FILE_FOR_KEY, 0);
			semid = connectToSem(firstKey, NUM_OF_SEM);
			msgidForAnswer = connectToMsg(firstKey);

			key_t secondKey = getTheKey(FILE_FOR_KEY, 1);
			msgidForTransmission = connectToMsg(secondKey);

			HeaderMessageStruct header;
			Flag isAll = FALSE;
			while(isAll == FALSE)
			{
				result = scanHeader(&header, newsockfd);
				if(result == -1)
				{
					header.type = END;
					header.loginSize = 0;
					header.passwordSize = 0;
					header.dataSize = 0;
					isAll = serverFiniteStateMachine(&header, newsockfd);
					printf("Close connect. Pid of process: %d\n", getpid());
				}
				else
					isAll = serverFiniteStateMachine(&header, newsockfd);
			}

			deleteString(stringPid);
			deleteString(userLogin);
			close(generalFifo);

			//end of process work
			exit(0);
		}

		//close socket for this user n both process
		close(newsockfd);
	}

	return 0;
}
