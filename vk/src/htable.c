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
	ret->size = HTABLE_SIZE;
	ret->array = (HTableNode **)calloc(HTABLE_SIZE, sizeof(HTableNode *));

	return ret;
}

void deleteHTable(HTableMap * htableMap)
{
	#ifdef DEBUG_HTABLE
	printf("deleteHTable\n");
	#endif /* DEBUG_HTABLE */

	int i;
	for(i = 0; i < htableMap->size; i++)
		deleteHTableNode(htableMap->array[i]);

	free(htableMap->array);
	free(htableMap);
}

void deleteHTableNode(HTableNode * node)
{
	if(node != NULL)
	{
		deleteHTableNode(node->next);
		deleteNameType(node->name);
		free(node);
	}
}

void deleteNameType(NameType * name)
{
	deleteString(name);
}
/*
void insertToHTable(HTableMap * htableMap, HTableData * data)
{
	
}

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
