#include"libs.h"
#include"config.h"
#include"general_config.h"
#include"my_string.h"
#include"htable.h"
#include"btree.h"
#include"sem.h"
#include"fifo.h"

key_t getTheKey(const char * keyFileName)
{
	key_t key = ftok(keyFileName, 0);
	CHECK("ftok", key);

	return key;
}

int main()
{
	HTableMap * htableMap = createHTable();
	assert(htableMap);
	readHTableFromFile(htableMap, HTABLE_STORAGE);

	BTreeMap * btreeMap = createBTree();
	assert(btreeMap);

	key_t key = getTheKey(FILE_FOR_KEY);

	int semid = createSem(key);

	int fifo = createFIFO(FIFO);







	/*
	//для регистрации
					desired = findInHTable(htableMap, login);
					if(desired == NULL)	//if not founded means can create with this login
					{
						insertToHTable(htableMap, convertToHTableData(login, password));
						insertToBTree(btreeMap, convertToBTreeData(login, "ip", getpid()));
					}
					else
					{
					}

	//для входа
					desired = findInHTable(htableMap, login);

					if(desired == NULL)
					{
						printf("Login error: not founded\n");
					}
					else if(stringCompare(desired->password, password) == 0)
					{
						insertToBTree(btreeMap, convertToBTreeData(login, "ip", getpid()));
					}
					else
					{
					}
*/




	CHECK("semctl", semctl(semid, 0, IPC_RMID, 0));
	remove(FIFO);
	printHTable(htableMap);
	saveHTable(htableMap, HTABLE_STORAGE);
	deleteHTable(htableMap);
	deleteBTree(btreeMap);

	return 0;
}
