#include"libs.h"
#include"configure.h"
#include"my_string.h"
#include"console.h"
#include"print.h"

int main()
{
	String * string = createString();

	printGreeting();
	consoleFiniteStateMachine(INIT);

	Flag isAll = FALSE;
	do
	{
		scanString(string);

		#ifdef DEBUG_CONSOLE
		printf("In the main loop:\n");
		printString(string);
		#endif /* DEBUG_CONSOLE */

		isAll = consoleFiniteStateMachine(parseCommand(string));
	}
	while(isAll == FALSE);

	deleteString(string);

	return 0;
}
