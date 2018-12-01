#include"libs.h"
#include"configure.h"
#include"my_string.h"
#include"console.h"
#include"print.h"
#include"htable.h"

HTableMap * htableMap;

int main()
{
	String * string = createString();
	assert(string);

	htableMap = createHTable();
	assert(htableMap);
	readHTableFromFile(htableMap, HTABLE_STORAGE);

	printGreeting();

	Flag isAll = consoleFiniteStateMachine(INIT);
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

	printHTable(htableMap);
	saveHTable(htableMap, HTABLE_STORAGE);
	deleteString(string);
	deleteHTable(htableMap);

	return 0;
}
