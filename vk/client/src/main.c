#include"libs.h"
#include"general_config.h"
#include"my_string.h"
#include"console.h"
#include"print.h"
#include"menu.h"

int sockfd = 0;
char * ip;

int main(int argc, char **argv)
{
	ip = (char *)calloc(16, sizeof(char));

	if(argc == 2)
		strcpy(ip, argv[1]);
	else
		strcpy(ip, "127.0.0.1");

	setConnect(sockfd);

	String * string = createString();
	assert(string);

	printGreeting();

	Flag isAll = consoleFiniteStateMachine(INITIALIZATION);
	do
	{
		scanStringFromStream(STDIN, string, -1);
		isAll = consoleFiniteStateMachine(parseCommand(string));
	}
	while(isAll == FALSE);

	deleteString(string);
	sendViaNet(END);
	close(sockfd);

	free(ip);

	return 0;
}
