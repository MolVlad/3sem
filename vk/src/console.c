#include"libs.h"
#include"configure.h"
#include"my_string.h"
#include"console.h"
#include"print.h"
#include"menu.h"
#include"account.h"
#include"htable.h"

Flag consoleFiniteStateMachine(enum ConsoleCommand command)
{
	static enum ConsoleState state = WELCOME_PAGE;
	static Flag isAll = FALSE;
	Flag isOK;
#ifdef DEBUG_CONSOLE
	printf("consoleFiniteStateMachine\n");
	printConsoleState(state);
	printConsoleCommand(command);
	#endif /* DEBUG_CONSOLE */

	switch(state)
	{
		case WELCOME_PAGE:
			switch(command)
			{
				case INIT:
					printf("Do you have an account? (y/n) <exit> for close\n");
					return isAll;
				case EXIT:
					printGoodbye();
					isAll = TRUE;
					return isAll;
				case YES:
					isOK = checkAccount();
					if(isOK == TRUE)
					{
						printf("Success\n");
						state = MAIN_MENU;
					}
					else if(isOK == FALSE)
					{
						printf("Wrong login or password\n");
						state = WELCOME_PAGE;
					}
					break;
				case NO:
					isOK = createAccount();
					if(isOK == TRUE)
					{
						printf("Success\n");
						state = MAIN_MENU;
					}
					else if(isOK == FALSE)
					{
						printf("Already exits\n");
						state = WELCOME_PAGE;
					}
					break;
				default:
					printError();
					state = WELCOME_PAGE;
			}

			consoleFiniteStateMachine(INIT);

			break;
		case MAIN_MENU:
			switch(command)
			{
				case INIT:
					printMainMenuCommandList();
					state = MAIN_MENU;
					return isAll;
				case EXIT:
					printLogOut();
					state = WELCOME_PAGE;
					break;
				case SEND_MSG:
					sendMessage();
					state = MAIN_MENU;
					break;
				case READ_DIALOGUE:
					readDialogue();
					state = MAIN_MENU;
					break;
				case USERS_LIST:
					userList();
					printHTable(htableMap);
					state = MAIN_MENU;
					break;
				case DELETE_MSG:
					deleteMessage();
					state = MAIN_MENU;
					break;
				case CLEAR_HISTORY:
					clearHistory();
					state = MAIN_MENU;
					break;
				default:
					printError();
					state = MAIN_MENU;
			}

			consoleFiniteStateMachine(INIT);

			break;
	}

	return isAll;
}

enum ConsoleCommand parseCommand(String * string)
{
	assert(string);

	if(strcmp(string->data, "y") == 0)
		return YES;
	if(strcmp(string->data, "n") == 0)
		return NO;
	if(strcmp(string->data, "exit") == 0)
		return EXIT;
	if(strcmp(string->data, "send") == 0)
		return SEND_MSG;
	if(strcmp(string->data, "read") == 0)
		return READ_DIALOGUE;
	if(strcmp(string->data, "users") == 0)
		return USERS_LIST;
	if(strcmp(string->data, "delete") == 0)
		return DELETE_MSG;
	if(strcmp(string->data, "clear") == 0)
		return CLEAR_HISTORY;

	return ERROR;
}

void printConsoleState(enum ConsoleState state)
{
	printf("ConsoleState = ");

	switch(state)
	{
		case WELCOME_PAGE:
			printf("WELCOME_PAGE\n");
			break;
		case MAIN_MENU:
			printf("MAIN_MENU\n");
			break;
	}
}

void printConsoleCommand(enum ConsoleCommand command)
{
	printf("ConsoleCommand = ");

	switch(command)
	{
		case INIT:
			printf("INIT\n");
			break;
		case ERROR:
			printf("ERROR\n");
			break;
		case YES:
			printf("YES\n");
			break;
		case NO:
			printf("NO\n");
			break;
		case EXIT:
			printf("EXIT\n");
			break;
		case SEND_MSG:
			printf("SEND_MSG\n");
			break;
		case READ_DIALOGUE:
			printf("READ_DIALOGUE\n");
			break;
		case USERS_LIST:
			printf("USERS_LIST\n");
			break;
		case DELETE_MSG:
			printf("DELETE_MSG\n");
			break;
		case CLEAR_HISTORY:
			printf("CLEAR_HISTORY\n");
			break;
	}
}
