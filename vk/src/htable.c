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

void saveHTable(HTableMap * htableMap, const char * fileName)
{
	assert(htableMap);
	assert(fileName);

	#ifdef DEBUG_HTABLE
	printf("saveHTable\n");
	#endif /* DEBUG_HTABLE */

	FILE * file = fopen(fileName, "w");
	assert(file);

	int i;
	for(i = 0; i < HTABLE_SIZE; i++)
	{
		if(htableMap->array[i] != NULL)
			saveHTableNode(file, htableMap->array[i]);
	}

	#ifdef DEBUG_HTABLE
	printf("saveHTable successful\n");
	#endif /* DEBUG_HTABLE */
}

void saveHTableNode(FILE * file, HTableNode * node)
{
	assert(file);
	assert(node);

	#ifdef DEBUG_HTABLE
	printf("saveHTableNode\n");
	#endif /* DEBUG_HTABLE */

	if(node->nextInChain != NULL)
		printHTableNode(node->nextInChain);

	saveHTableNodeName(file, node->name);
	saveHTableNodeData(file, node->data);

	#ifdef DEBUG_HTABLE
	printf("saveHTableNode successful\n");
	#endif /* DEBUG_HTABLE */
}

void saveHTableNodeName(FILE * file, NameType * name)
{
	assert(file);
	assert(name);

	printStringToFile(file, name);
}

void saveHTableNodeData(FILE * file, HTableData * data)
{
	assert(file);
	assert(data);

	printStringToFile(file, data->login);
	printStringToFile(file, data->password);
}

void readHTableFromFile(HTableMap * htableMap, const char * fileName)
{
	assert(htableMap);
	assert(fileName);

	#ifdef DEBUG_HTABLE
	printf("readHTableFromFile\n");
	#endif /* DEBUG_HTABLE */

	FILE * file = fopen(fileName, "r");
	assert(file);

	String * login = createString();
	assert(login);
	String * password = createString();
	assert(login);



	Flag isAll = FALSE;
	while(isAll == FALSE)
	{
		
	}

	#ifdef DEBUG_HTABLE
	printf("readHTableFromFile successful\n");
	#endif /* DEBUG_HTABLE */
}

Flag areNamesSame(NameType * first, NameType * second)
{
	assert(first);
	assert(second);

	return areStringSame(first, second);
}

HTableData * findHTableNodeInChein(HTableNode * compared, NameType * name)
{
	assert(name);

	#ifdef DEBUG_HTABLE
	printf("compareHTableNodeName\n");
	#endif /* DEBUG_HTABLE */

	if(compared == NULL)
		return NULL;

	if(areNamesSame(compared->name, name) == TRUE)
		return compared->data;

	if(compared->nextInChain != NULL)
		return findHTableNodeInChein(compared->nextInChain, name);

	return NULL;
}

HTableData * findInHTable(HTableMap * htableMap, NameType * name)
{
	assert(name);

	#ifdef DEBUG_HTABLE
	printf("findInHTable\n");
	#endif /* DEBUG_HTABLE */

	HashType hash = hashFunction(name);

	HTableData * ret = findHTableNodeInChein(htableMap->array[hash], name);

	#ifdef DEBUG_HTABLE
	printf("findInHTable: is it found = %d\n", (ret != NULL));
	#endif /* DEBUG_HTABLE */

	return ret;
}
