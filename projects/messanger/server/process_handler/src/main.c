#include"libs.h"
#include"general_config.h"
#include"my_string.h"
#include"database_communication.h"
#include"user_communication.h"
#include"fight_zombie_process.h"
#include"server_state_machines.h"
#include"fifo_config.h"
#include"msg_queue_config.h"
#include"sem_config.h"
#include"server_msg_types.h"
#include"signal_handlers.h"
pthread_t thid = 0; int handlerPid;
int generalFifo;
int semid;
int msgidForAnswer;
int msgidForTransmission;
int sockfd;
String * stringPid;
String * userLogin;
char privateFifoName[PRIVATE_FIFO_NAME_SIZE];

int main()
{
	configSignals();

	sockfd = raiseServer();

	int result = createThreadToFightZombie();
	CHECK("createThreadToFightZombie", result);

	handlerPid = fork();
	CHECK("fork", handlerPid);
	if(handlerPid == 0)
	{
		result = execlp("database/database", "database", NULL);
		CHECK("execlp", result);
	}

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
			msgidForAnswer = connectToMsgQueue(firstKey);

			key_t secondKey = getTheKey(FILE_FOR_KEY, 1);
			msgidForTransmission = connectToMsgQueue(secondKey);

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
