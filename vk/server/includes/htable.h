#ifndef __HTABLE_H__
#define __HTABLE_H__

typedef struct
{
	String * login;
	String * password;

} HTableData;

typedef unsigned int HashType;

typedef struct HTableNode
{
	struct HTableNode * nextInChain;
	HTableData * data;
} HTableNode;

typedef struct
{
	HTableNode ** array;
	int size;
} HTableMap;

extern HTableMap * htableMap;

// Parameters for hash-function
#define HASH_FUNCTION_INIT_VALUE 2723797
#define HASH_FUNCTION_PRIME_NUMBER 2462741

// Function for use
HTableMap * createHTable();
void deleteHTable(HTableMap * htableMap);
void insertToHTable(HTableMap * htableMap, HTableData * data);
void printHTable(HTableMap * htableMap);
void saveHTable(HTableMap * htableMap, const char * fileName);
HTableData * findInHTable(HTableMap * htableMap, String * login);
void readHTableFromFile(HTableMap * htableMap, const char * fileName);

// Private function
void deleteHTableNode(HTableNode * node);
void deleteHTableData(HTableData * data);
int putInHTable(HTableMap * htableMap, HashType hash, HTableNode * node);
HashType hashFunction(String * login);
HTableData * convertToHTableData(String * login, String * password);
HTableNode * createHTableNode(HTableData * data);
void printHTableNode(HTableNode * node);
void printHTableNodeData(HTableData * data);
void saveHTableNode(int fd, HTableNode * node);
void saveHTableNodeData(int fd, HTableData * data);
HTableData * findHTableNodeInChein(HTableNode * compared, String * login);

#endif /* __HTABLE_H__ */
