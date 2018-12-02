#include"libs.h"
#include"configure.h"
#include"my_string.h"
#include"console.h"
#include"print.h"
#include"htable.h"
#include"btree.h"

HTableMap * htableMap = NULL;
BTreeMap * btreeMap = NULL;

int main()
{
	String * string = createString();
	assert(string);

	htableMap = createHTable();
	assert(htableMap);
	readHTableFromFile(htableMap, HTABLE_STORAGE);

	btreeMap = createBTree();
	assert(btreeMap);

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
	printBTree(btreeMap);

	deleteString(string);
	deleteHTable(htableMap);
	deleteBTree(btreeMap);

	return 0;
}
