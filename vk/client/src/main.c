#include"libs.h"
#include"config.h"
#include"general_config.h"
#include"my_string.h"
#include"console.h"
#include"print.h"
#include"menu.h"
#include"sem.h"

int semid;
int sockfd;
char * ip;

void sigHandler(int nsig)
{
	printf("Print to stream error with SIGPIPE. nsig = %d\n", nsig);
	exit(-1);
}

void *thread()
{
	printf("lol\n");
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

int main(int argc, char **argv)
{
	(void) signal(SIGPIPE, sigHandler);

	ip = (char *)calloc(SIZE_IP, sizeof(char));

	if(argc == 2)
		strcpy(ip, argv[1]);
	else
		strcpy(ip, DEFAULT_IP);

	key_t key = getTheKey(FILE_FOR_KEY);
	semid = createSem(key, NUM_OF_SEM);
	semOperation(semid, communicationWithServer, 1);

	pthread_t thid;
	int  result = pthread_create(&thid, NULL, thread, NULL);
	if(result != 0)
	{
		perror("pthread create");
		exit(-1);
	}

	setConnect(sockfd);

	String * string = createString();
	assert(string);

	printGreeting();

	Flag isAll = consoleFiniteStateMachine(INITIALIZATION);
	do
	{
		scanStringFromStream(STDIN, string, -1);
		semOperation(semid, communicationWithServer, -1);
		printf("main loop\n");
		isAll = consoleFiniteStateMachine(parseCommand(string));
		semOperation(semid, communicationWithServer, 1);
	}
	while(isAll == FALSE);

	deleteString(string);
	sendViaNet(END);
	CHECK("semctl", semctl(semid, 0, IPC_RMID, 0));
	close(sockfd);

	free(ip);

	return 0;
}
