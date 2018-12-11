#include"libs.h"
#include"config.h"
#include"general_config.h"
#include"my_string.h"
#include"handler_communication.h"
#include"user_communication.h"
#include"zombie.h"
#include"state.h"
#include"fifo.h"

#define REQUEST_NUMBER 5

int main()
{
	int sockfd = raiseServer();

	int result = createThreadToFightZombie();
	CHECK("createThreadToFightZombie", result);

	if(fork() == 0)
		execlp("handler/handler", "handler", NULL);

	//for debug via valgrind
	//int k = REQUEST_NUMBER;
	//while(k--)
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

			int fifo = openFIFO(FIFO);

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
					isAll = serverFiniteStateMachine(&header, newsockfd, fifo);
			}

			close(fifo);

			//end of process work
			exit(0);
		}

		//close socket for this user n both process
		close(newsockfd);
	}

	//save list of logins/passwords and free memory
	close(sockfd);

	return 0;
}
