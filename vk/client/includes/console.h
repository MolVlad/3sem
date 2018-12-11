#ifndef __CONSOLE_STATE_H__
#define __CONSOLE_STATE_H__

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

#endif /* __CONSOLE_STATE_H__ */
