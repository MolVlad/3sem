#include"libs.h"
#include"config.h"
#include"general_config.h"
#include"my_string.h"
#include"htable.h"

HTableMap * createHTable()
{
	HTableMap * ret = (HTableMap *)calloc(1, sizeof(HTableMap));
	assert(ret);

	ret->size = HTABLE_SIZE;
	ret->array = (HTableNode **)calloc(HTABLE_SIZE, sizeof(HTableNode *));
	assert(ret->array);

	return ret;
}

void insertToHTable(HTableMap * htableMap, HTableData * data)
{
	assert(htableMap);
	assert(data);

	HashType hash = hashFunction(data->login);

	HTableNode * node = createHTableNode(data);
	assert(node);
	putInHTable(htableMap, hash, node);
}

void printHTable(HTableMap * htableMap)
{
	assert(htableMap);

	int i;
	for(i = 0; i < HTABLE_SIZE; i++)
	{
		if(htableMap->array[i] != NULL)
			printHTableNode(htableMap->array[i]);
	}
}

void printHTableNode(HTableNode * node)
{
	assert(node);

	if(node->nextInChain != NULL)
		printHTableNode(node->nextInChain);

	printHTableNodeData(node->data);
}

void printHTableNodeData(HTableData * data)
{
	assert(data);

	printf("login:\n");
	printStringToStream(STDOUT, data->login);
	printf("\n");
	printf("password:\n");
	printStringToStream(STDOUT, data->password);
	printf("\n");
}

void deleteHTable(HTableMap * htableMap)
{
	assert(htableMap);

	int i, size = htableMap->size;
	for(i = 0; i < size; i++)
		if(htableMap->array[i] != NULL)
			deleteHTableNode(htableMap->array[i]);

	free(htableMap->array);
	free(htableMap);
}

void deleteHTableNode(HTableNode * node)
{
	assert(node);

	if(node->nextInChain != NULL)
		deleteHTableNode(node->nextInChain);

	deleteHTableData(node->data);
	free(node);
}

void deleteHTableData(HTableData * data)
{
	assert(data);

	deleteString(data->login);
	deleteString(data->password);

	free(data);
}

HashType hashFunction(String * login)
{
	assert(login);

	HashType hash;

	hash = HASH_FUNCTION_INIT_VALUE;

	int i;
	for(i = 0; i < login->currentSize; i++)
		hash += login->data[i] * HASH_FUNCTION_PRIME_NUMBER;

	hash = hash % HTABLE_SIZE;

	return hash;
}

HTableNode * createHTableNode(HTableData * data)
{
	assert(data);

	HTableNode * ret = (HTableNode *)calloc(1, sizeof(HTableNode));
	assert(ret);

	ret->data = data;

	return ret;
}

int putInHTable(HTableMap * htableMap, HashType hash, HTableNode * node)
{
	assert(htableMap);
	assert(node);

	HTableNode * pointer = htableMap->array[hash];
	if(pointer == NULL)
	{
		htableMap->array[hash] = node;
		return 0;
	}

	HTableNode * prev;
	while(pointer != NULL)
	{
		prev = pointer;
		pointer = pointer->nextInChain;
	}
	prev->nextInChain = node;

	return 0;
}

HTableData * convertToHTableData(String * login, String * password)
{
	assert(login);
	assert(password);

	HTableData * ret = (HTableData *)calloc(1, sizeof(HTableData));
	assert(ret);

	ret->login = copyString(login);
	assert(ret->login);
	ret->password = copyString(password);
	assert(ret->password);

	return ret;
}

void saveHTable(HTableMap * htableMap, const char * fileName)
{
	assert(htableMap);
	assert(fileName);

	int fd = open(fileName, O_WRONLY);
	CHECK("open", fd);

	int i;
	for(i = 0; i < HTABLE_SIZE; i++)
	{
		if(htableMap->array[i] != NULL)
			saveHTableNode(fd, htableMap->array[i]);
	}

	printf("Htable saved\n");

	close(fd);
}

void saveHTableNode(int fd, HTableNode * node)
{
	assert(node);

	if(node->nextInChain != NULL)
		saveHTableNode(fd, node->nextInChain);

	saveHTableNodeData(fd, node->data);
}

void saveHTableNodeData(int fd, HTableData * data)
{
	assert(data);

	printStringToStream(fd, data->login);
	write(fd, "\n", 1);
	printStringToStream(fd, data->password);
	write(fd, "\n", 1);
}

void readHTableFromFile(HTableMap * htableMap, const char * fileName)
{
	assert(htableMap);
	assert(fileName);

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
		result = scanStringFromStream(fd, login, -1);
		result = scanStringFromStream(fd, password, -1);

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
}

HTableData * findHTableNodeInChein(HTableNode * compared, String * login)
{
	assert(login);

	if(compared == NULL)
		return NULL;

	if(stringCompare(compared->data->login, login) == 0)
		return compared->data;

	if(compared->nextInChain != NULL)
		return findHTableNodeInChein(compared->nextInChain, login);

	return NULL;
}

HTableData * findInHTable(HTableMap * htableMap, String * login)
{
	assert(login);
	assert(htableMap);

	HashType hash = hashFunction(login);

	HTableData * ret = findHTableNodeInChein(htableMap->array[hash], login);

	return ret;
}
