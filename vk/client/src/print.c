#include"libs.h"
#include"general_config.h"
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
	printf("<users>\t\tprint list of users\n");
	printf("<exit>\t\tlog out\n");
	printf("\n");
}

void printLogOut()
{
	printf("Log out\n");
}
