#ifndef __STATE_MACHINES_H__
#define __STATE_MACHINES_H__

enum ConsoleState
{
	WELCOME_PAGE = 0,
	MAIN_MENU,
};

enum ConsoleCommand
{
	INITIALIZATION,
	ERROR,

	EXIT,
	YES,
	NO,
	SEND_MSG,
	USERS_LIST,
};

Flag consoleFiniteStateMachine(enum ConsoleCommand command);

enum ConsoleCommand parseCommand(String * string);
void printMainMenuCommandList();

#endif /* __STATE_MACHINES_H__ */
