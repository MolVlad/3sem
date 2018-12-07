#include"libs.h"
#include"configure.h"
#include"my_string.h"
#include"htable.h"

HTableMap * createHTable()
{
	#ifdef DEBUG_HTABLE
	printf("createHTable\n");
	#endif /* DEBUG_HTABLE */

	HTableMap * ret = (HTableMap *)calloc(1, sizeof(HTableMap));
	assert(ret);

	ret->size = HTABLE_SIZE;
	ret->array = (HTableNode **)calloc(HTABLE_SIZE, sizeof(HTableNode *));
	assert(ret->array);

	#ifdef DEBUG_HTABLE
	printf("createHTable successful\n");
	#endif /* DEBUG_HTABLE */

	return ret;
}

void insertToHTable(HTableMap * htableMap, HTableData * data)
{
	assert(htableMap);
	assert(data);

	#ifdef DEBUG_HTABLE
	printf("insertToHTable\n");
	#endif /* DEBUG_HTABLE */

	HashType hash = hashFunction(data->login);

	HTableNode * node = createHTableNode(data);
	assert(node);
	putInHTable(htableMap, hash, node);

	#ifdef DEBUG_HTABLE
	printf("insertToHTable successful\n");
	#endif /* DEBUG_HTABLE */
}

void printHTable(HTableMap * htableMap)
{
	assert(htableMap);

	#ifdef DEBUG_HTABLE
	printf("printHTable\n");
	#endif /* DEBUG_HTABLE */

	int i;
	for(i = 0; i < HTABLE_SIZE; i++)
	{
		if(htableMap->array[i] != NULL)
			printHTableNode(htableMap->array[i]);
	}

	#ifdef DEBUG_HTABLE
	printf("printHTable successful\n");
	#endif /* DEBUG_HTABLE */
}

void printHTableNode(HTableNode * node)
{
	assert(node);

	#ifdef DEBUG_HTABLE
	printf("printHTableNode\n");
	#endif /* DEBUG_HTABLE */

	if(node->nextInChain != NULL)
		printHTableNode(node->nextInChain);

	printHTableNodeData(node->data);

	#ifdef DEBUG_HTABLE
	printf("printHTableNode successful\n");
	#endif /* DEBUG_HTABLE */
}

void deleteHTable(HTableMap * htableMap)
{
	assert(htableMap);

	#ifdef DEBUG_HTABLE
	printf("deleteHTable\n");
	#endif /* DEBUG_HTABLE */

	int i, size = htableMap->size;
	for(i = 0; i < size; i++)
		if(htableMap->array[i] != NULL)
			deleteHTableNode(htableMap->array[i]);

	free(htableMap->array);
	free(htableMap);

	#ifdef DEBUG_HTABLE
	printf("deleteHTable successful\n");
	#endif /* DEBUG_HTABLE */
}

void deleteHTableNode(HTableNode * node)
{
	assert(node);

	#ifdef DEBUG_HTABLE
	printf("deleteHTableNode\n");
	#endif /* DEBUG_HTABLE */

	if(node->nextInChain != NULL)
		deleteHTableNode(node->nextInChain);

	deleteHTableData(node->data);
	free(node);

	#ifdef DEBUG_HTABLE
	printf("deleteHTableNode successful\n");
	#endif /* DEBUG_HTABLE */
}

void deleteHTableData(HTableData * data)
{
	assert(data);

	#ifdef DEBUG_HTABLE
	printf("deleteHTableData\n");
	#endif /* DEBUG_HTABLE */

	deleteString(data->login);
	deleteString(data->password);

	free(data);

	#ifdef DEBUG_HTABLE
	printf("deleteHTableData successful\n");
	#endif /* DEBUG_HTABLE */
}

HashType hashFunction(String * login)
{
	assert(login);

	#ifdef DEBUG_HTABLE
	printf("hashFunction\n");
	#endif /* DEBUG_HTABLE */

	HashType hash;

	hash = HASH_FUNCTION_INIT_VALUE;

	int i;
	for(i = 0; i < login->currentSize; i++)
		hash += login->data[i] * HASH_FUNCTION_PRIME_NUMBER;

	hash = hash % HTABLE_SIZE;

	#ifdef DEBUG_HTABLE
	printf("hash = %d\n", hash);
	#endif /* DEBUG_HTABLE */

	return hash;
}

HTableNode * createHTableNode(HTableData * data)
{
	assert(data);

	#ifdef DEBUG_HTABLE
	printf("createHTableNode\n");
	#endif /* DEBUG_HTABLE */

	HTableNode * ret = (HTableNode *)calloc(1, sizeof(HTableNode));
	assert(ret);

	ret->data = data;

	#ifdef DEBUG_HTABLE
	printf("createHTableNode successful\n");
	#endif /* DEBUG_HTABLE */

	return ret;
}

int putInHTable(HTableMap * htableMap, HashType hash, HTableNode * node)
{
	assert(htableMap);
	assert(node);

	#ifdef DEBUG_HTABLE
	printf("putInHTable\n");
	#endif /* DEBUG_HTABLE */

	HTableNode * pointer = htableMap->array[hash];
	if(pointer == NULL)
	{
		htableMap->array[hash] = node;
		return 0;
	}

	HTableNode * prev;
	while(pointer != NULL)
	{
		#ifdef DEBUG_HTABLE
		printf("Move to next\n");
		#endif /* DEBUG_HTABLE */

		prev = pointer;
		pointer = pointer->nextInChain;
	}
	prev->nextInChain = node;

	#ifdef DEBUG_HTABLE
	printf("putInHTable successful\n");
	#endif /* DEBUG_HTABLE */

	return 0;
}

HTableData * convertToHTableData(String * login, String * password)
{
	assert(login);
	assert(password);

	#ifdef DEBUG_HTABLE
	printf("convertToHTableData\n");
	#endif /* DEBUG_HTABLE */

	HTableData * ret = (HTableData *)calloc(1, sizeof(HTableData));
	assert(ret);

	ret->login = copyString(login);
	assert(ret->login);
	ret->password = copyString(password);
	assert(ret->password);

	#ifdef DEBUG_HTABLE
	printf("convertToHTableData successful\n");
	#endif /* DEBUG_HTABLE */

	return ret;
}

void printHTableNodeData(HTableData * data)
{
	assert(data);

	printf("login:\n");
	printStringToStream(STDOUT, data->login);
	printf("password:\n");
	printStringToStream(STDOUT, data->password);
}

void saveHTable(HTableMap * htableMap, const char * fileName)
{
	assert(htableMap);
	assert(fileName);

	#ifdef DEBUG_HTABLE
	printf("saveHTable\n");
	#endif /* DEBUG_HTABLE */

	int fd = open(fileName, O_WRONLY);
	CHECK("open", fd);

	int i;
	for(i = 0; i < HTABLE_SIZE; i++)
	{
		if(htableMap->array[i] != NULL)
			saveHTableNode(fd, htableMap->array[i]);
	}

	close(fd);

	#ifdef DEBUG_HTABLE
	printf("saveHTable successful\n");
	#endif /* DEBUG_HTABLE */
}

void saveHTableNode(int fd, HTableNode * node)
{
	assert(node);

	#ifdef DEBUG_HTABLE
	printf("saveHTableNode\n");
	#endif /* DEBUG_HTABLE */

	if(node->nextInChain != NULL)
		saveHTableNode(fd, node->nextInChain);

	saveHTableNodeData(fd, node->data);

	#ifdef DEBUG_HTABLE
	printf("saveHTableNode successful\n");
	#endif /* DEBUG_HTABLE */
}

void saveHTableNodeData(int fd, HTableData * data)
{
	assert(data);

	printStringToStream(fd, data->login);
	printStringToStream(fd, data->password);
}

void readHTableFromFile(HTableMap * htableMap, const char * fileName)
{
	assert(htableMap);
	assert(fileName);

	#ifdef DEBUG_HTABLE
	printf("readHTableFromFile\n");
	#endif /* DEBUG_HTABLE */

	int fd = open(fileName, O_RDONLY);
	CHECK("open", fd);

	String * login = createString();
	assert(login);
	String * password = createString();
	assert(login);

	HTableData * desired;

	int result;
	Flag isAll = FALSE;
	while(isAll == FALSE)
	{
		result = scanStringFromStream(fd, login);
		result = scanStringFromStream(fd, password);

		#ifdef DEBUG_HTABLE
		printf("scanned:\n");
		printf("login:\n");
		printStringToStream(STDOUT, login);
		printf("password:\n");
		printStringToStream(STDOUT, password);
		#endif /* DEBUG_HTABLE */

		if(result == -1)
			isAll = TRUE;
		else
		{
			desired = findInHTable(htableMap, login);
			if(desired == NULL)
				insertToHTable(htableMap, convertToHTableData(login, password));
		}
	}

	deleteString(login);
	deleteString(password);
	close(fd);

	#ifdef DEBUG_HTABLE
	printf("readHTableFromFile successful\n");
	#endif /* DEBUG_HTABLE */
}

HTableData * findHTableNodeInChein(HTableNode * compared, String * login)
{
	assert(login);

	#ifdef DEBUG_HTABLE
	printf("compareHTableNodeName\n");
	#endif /* DEBUG_HTABLE */

	if(compared == NULL)
		return NULL;

	if(areStringSame(compared->data->login, login) == TRUE)
		return compared->data;

	if(compared->nextInChain != NULL)
		return findHTableNodeInChein(compared->nextInChain, login);

	return NULL;
}

HTableData * findInHTable(HTableMap * htableMap, String * login)
{
	assert(login);

	#ifdef DEBUG_HTABLE
	printf("findInHTable\n");
	#endif /* DEBUG_HTABLE */

	HashType hash = hashFunction(login);

	HTableData * ret = findHTableNodeInChein(htableMap->array[hash], login);

	#ifdef DEBUG_HTABLE
	printf("findInHTable: is it found = %d\n", (ret != NULL));
	#endif /* DEBUG_HTABLE */

	return ret;
}
