#include"libs.h"
#include"config.h"
#include"general_config.h"
#include"my_string.h"
#include"console.h"
#include"print.h"
#include"menu.h"
#include"sem.h"
#include"thread.h"

int semid;
int sockfd;
char * ip;
pthread_t thid;

int main(int argc, char **argv)
{
	(void) signal(SIGPIPE, sigHandler);
	(void) signal(SIGUSR1, threadSidHandler);

	ip = (char *)calloc(SIZE_IP, sizeof(char));

	if(argc == 2)
		strcpy(ip, argv[1]);
	else
		strcpy(ip, DEFAULT_IP);

	key_t key = getTheKey("client");
	semid = createSem(key, NUM_OF_SEM);
	semOperation(semid, communicationWithServer, 1);

	setConnect(sockfd);

	String * string = createString();
	assert(string);

	printGreeting();

	Flag isAll = consoleFiniteStateMachine(INITIALIZATION);
	do
	{
		scanStringFromStream(STDIN, string, -1);
		semOperation(semid, communicationWithServer, -1);
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
