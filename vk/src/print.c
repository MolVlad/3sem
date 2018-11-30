#include"libs.h"
#include"configure.h"
#include"print.h"

void printGreeting()
{
	printf("Hello!\n");
}

void printGoodbye()
{
	printf("See you soon\n");
}

void printError()
{
	printf("Wrong command\n");
}

void printMainMenuCommandList()
{
	printf("\nCommands:\n");
	printf("<send>\t\twrite a message to someone\n");
	printf("<read>\t\tprint dialogue with someone\n");
	printf("<users>\t\tprint list of users\n");
	printf("<delete>\t\tremove some message\n");
	printf("<clear>\t\tclear char with someone\n");
	printf("<exit>\t\tlog out\n");
	printf("\n");
}

void printLogOut()
{
	printf("Log out\n");
}
