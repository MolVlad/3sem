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
	READ_DIALOGUE,
	USERS_LIST,
	DELETE_MSG,
	CLEAR_HISTORY,
};

Flag consoleFiniteStateMachine(enum ConsoleCommand command);
enum ConsoleCommand parseCommand(String * string);
void printConsoleState(enum ConsoleState state);
void printConsoleCommand(enum ConsoleCommand command);

#endif /* __CONSOLE_STATE_H__ */
