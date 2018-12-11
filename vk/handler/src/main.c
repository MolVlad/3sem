#include"libs.h"
#include"config.h"
#include"general_config.h"
#include"my_string.h"
#include"htable.h"
#include"btree.h"
#include"sem.h"
#include"fifo.h"

HTableMap * htableMap;

key_t getTheKey(const char * keyFileName)
{
	key_t key = ftok(keyFileName, 0);
	CHECK("ftok", key);

	return key;
}

void sigHandler(int nsig)
{
	printf("Exit from handler with signal SIGINT, nsig = %d\n", nsig);
	saveHTable(htableMap, HTABLE_STORAGE);
	exit(0);
}

int main()
{
	(void) signal(SIGINT, sigHandler);

	htableMap = createHTable();
	assert(htableMap);
	readHTableFromFile(htableMap, HTABLE_STORAGE);

	BTreeMap * btreeMap = createBTree();
	assert(btreeMap);

	key_t key = getTheKey(FILE_FOR_KEY);

	int semid = createSem(key);

	int fifo = createFIFO(FIFO);

	while(1);





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
	deleteHTable(htableMap);
	deleteBTree(btreeMap);

	return 0;
}
