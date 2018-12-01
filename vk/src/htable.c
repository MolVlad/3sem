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

	NameType * name = copyName(data->login);
	assert(name);

	HTableNode * node = createHTableNode(name, data);
	assert(node);
	putInHTable(htableMap, hash, node);

	#ifdef DEBUG_HTABLE
	printf("insertToHTable successful\n");
	#endif /* DEBUG_HTABLE */
}

void printHTable(HTableMap * htableMap)
{
	#ifdef DEBUG_HTABLE
	printf("printHTable\n");
	#endif /* DEBUG_HTABLE */

	assert(htableMap);

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

	printf("Node name:\n");
	printHTableNodeName(node->name);
	printf("Node data:\n");
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

	int i;
	for(i = 0; i < htableMap->size; i++)
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
	#ifdef DEBUG_HTABLE
	printf("deleteHTableNode\n");
	#endif /* DEBUG_HTABLE */

	assert(node);

	if(node->nextInChain)
		deleteHTableNode(node->nextInChain);

	deleteNameType(node->name);
	free(node);

	#ifdef DEBUG_HTABLE
	printf("deleteHTableNode successful\n");
	#endif /* DEBUG_HTABLE */
}

void deleteNameType(NameType * name)
{
	#ifdef DEBUG_HTABLE
	printf("deleteNameType\n");
	#endif /* DEBUG_HTABLE */

	assert(name);

	deleteString(name);

	#ifdef DEBUG_HTABLE
	printf("deleteNameType successful\n");
	#endif /* DEBUG_HTABLE */
}

HashType hashFunction(NameType * name)
{
	assert(name);

	#ifdef DEBUG_HTABLE
	printf("hashFunction\n");
	#endif /* DEBUG_HTABLE */

	HashType hash;

	hash = HASH_FUNCTION_INIT_VALUE;

	int i;
	for(i = 0; i < name->currentSize; i++)
		hash += name->data[i] * HASH_FUNCTION_PRIME_NUMBER;

	hash = hash % HTABLE_SIZE;

	#ifdef DEBUG_HTABLE
	printf("hash = %d\n", hash);
	#endif /* DEBUG_HTABLE */

	return hash;
}

HTableNode * createHTableNode(NameType * name, HTableData * data)
{
	assert(name);
	assert(data);

	#ifdef DEBUG_HTABLE
	printf("createHTableNode\n");
	#endif /* DEBUG_HTABLE */

	HTableNode * ret = (HTableNode *)calloc(1, sizeof(HTableNode));
	assert(ret);

	ret->name = name;
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

NameType * copyName(NameType * original)
{
	assert(original);

	#ifdef DEBUG_HTABLE
	printf("copyName\n");
	#endif /* DEBUG_HTABLE */

	NameType * ret = copyString(original);
	assert(ret);

	#ifdef DEBUG_HTABLE
	printf("copyName successful\n");
	#endif /* DEBUG_HTABLE */

	return ret;
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

void printHTableNodeName(NameType * name)
{
	assert(name);

	printString(name);
}

void printHTableNodeData(HTableData * data)
{
	assert(data);

	printf("login = ");
	printString(data->login);
	printf("password = ");
	printString(data->password);
}

/*

HTableData * findInHTable(HTableMap * htableMap, NameType * name)
{
}

void removeFromHTable(HTableMap * htableMap, NameType * name)
{
}

void readHTableFromFile(HTableMap * htableMap, const char * fileName)
{
}

void saveHTable(HTableMap * htableMap, const char * fileName)
{
}*/
