#include"libs.h"
#include"configure.h"
#include"my_string.h"
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

	printHTable(htableMap);
	saveHTable(htableMap, HTABLE_STORAGE);
	printBTree(btreeMap);

	deleteString(string);
	deleteHTable(htableMap);
	deleteBTree(btreeMap);

	return 0;
}