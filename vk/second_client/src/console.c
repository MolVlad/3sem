#include"libs.h"
#include"config.h"
#include"general_config.h"
#include"my_string.h"
#include"console.h"
#include"print.h"
#include"menu.h"
#include"global.h"
#include"sem.h"
#include"thread.h"

Flag consoleFiniteStateMachine(enum ConsoleCommand command)
{
	static enum ConsoleState state = WELCOME_PAGE;
	static Flag isAll = FALSE;
	Flag isOK;

	switch(state)
	{
		case WELCOME_PAGE:
			switch(command)
			{
				case INITIALIZATION:
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
						//create new thread
						createThread();
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
						//create new thread
						createThread();
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

			consoleFiniteStateMachine(INITIALIZATION);

			break;
		case MAIN_MENU:
			switch(command)
			{
				case INITIALIZATION:
					printMainMenuCommandList();
					state = MAIN_MENU;
					return isAll;
				case EXIT:
					logOut(sockfd);
					deleteThread();
					//reconnect
					setConnect();
					state = WELCOME_PAGE;
					break;
				case SEND_MSG:
					isOK = sendMessage();
					if(isOK == TRUE)
					{
						printf("Success\n");
						state = MAIN_MENU;
					}
					else if(isOK == FALSE)
					{
						printf("User is unavailable\n");
						state = WELCOME_PAGE;
					}
					state = MAIN_MENU;
					break;
				case USERS_LIST:
					userList();
					state = MAIN_MENU;
					break;
				default:
					printError();
					state = MAIN_MENU;
			}

			consoleFiniteStateMachine(INITIALIZATION);

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
	if(strcmp(string->data, "users") == 0)
		return USERS_LIST;

	return ERROR;
}
