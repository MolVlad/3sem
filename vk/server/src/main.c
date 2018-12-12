#include"libs.h"
#include"config.h"
#include"general_config.h"
#include"config_serv_hand.h"
#include"my_string.h"
#include"handler_communication.h"
#include"user_communication.h"
#include"zombie.h"
#include"state.h"
#include"fifo.h"
#include"msg.h"
#include"sem.h"

#define REQUEST_NUMBER 5

int handlerPid;
int fifo;
int semid;
int msgidForAnswers;
int msgidForMessages;
int sockfd;
String * stringPid;
String * userLogin;

void sigHandler(int nsig)
{
	printf("Exit from server with signal SIGINT, nsig = %d\n", nsig);
	kill(handlerPid, SIGINT);
	close(sockfd);
	exit(0);
}

int main()
{
	(void) signal(SIGINT, sigHandler);

	sockfd = raiseServer();

	int result = createThreadToFightZombie();
	CHECK("createThreadToFightZombie", result);

	handlerPid = fork();
	if(handlerPid == 0)
		execlp("../handler/handler", "handler", NULL);

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

			fifo = openFIFO(FIFO);
			key_t firstKey = getTheKey(FIRST_FILE_FOR_KEY);
			key_t secondKey = getTheKey(SECOND_FILE_FOR_KEY);
			semid = connectToSem(firstKey, NUM_OF_SEM);
			msgidForAnswers = connectToMsg(firstKey);
			msgidForMessages = connectToMsg(secondKey);

			/////////нужна отдельная нить, которая будет слушать из другой очереди сообщений
			/////////нужен семафор для синхронизации отправки пользователю
			HeaderMessageStruct header;
			Flag isAll = FALSE;
			while(isAll == FALSE)
			{
				result = scanHeader(&header, newsockfd);
				if(result == -1)
				{
					printf("Close connect. Pid of process: %d\n", getpid());
					isAll = TRUE;
				}
				else
					isAll = serverFiniteStateMachine(&header, newsockfd);
			}

			deleteString(stringPid);
			deleteString(userLogin);
			close(fifo);

			//end of process work
			exit(0);
		}

		//close socket for this user n both process
		close(newsockfd);
	}

	return 0;
}
