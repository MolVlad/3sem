#include"libs.h"
#include"default_config.h"
#include"general_config.h"

#include"my_string.h"
#include"sem_config.h"
#include"state_machines.h"
#include"server_api.h"
#include"server_connection.h"
#include"server_feedback.h"
#include"signal_handlers.h"
#include"threads.h"

int semid;
int sockfd;
char * ip;
pthread_t thid;
String * stringKey;

int main(int argc, char **argv)
{
	int result;

	configSignals();
	ip = (char *)calloc(SIZE_IP, sizeof(char));

	if(argc == 2)
		strcpy(ip, argv[1]);
	else
		strcpy(ip, DEFAULT_IP);

	result = setConnect(sockfd);
	CHECK("setConnect", result);

	stringKey = pidToString();
	result = mkfifo(stringKey->data, PERMISSION);
	CHECK("create", result);
	key_t key = getTheKey(stringKey->data, 0);

	semid = createSem(key, NUM_OF_SEM);
	semOperation(semid, communicationWithServer, 1);

	String * string = createString();
	assert(string);

	printf("Hello!\n");

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
	remove(stringKey->data);
	deleteString(stringKey);
	CHECK("semctl", semctl(semid, 0, IPC_RMID, 0));
	close(sockfd);

	free(ip);

	return 0;
}
