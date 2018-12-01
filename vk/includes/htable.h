#ifndef __HTABLE_H__
#define __HTABLE_H__

typedef struct
{
	String * login;
	String * password;

} HTableData;

typedef unsigned int HashType;
typedef String NameType;

typedef struct HTableNode
{
	NameType * name;
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

// Private function
void deleteHTableNode(HTableNode * node);
void deleteNameType(NameType * name);
int putInHTable(HTableMap * htableMap, HashType hash, HTableNode * node);
NameType * copyName(NameType * original);
HashType hashFunction(NameType * name);
HTableData * convertToHTableData(String * login, String * password);
HTableNode * createHTableNode(NameType * name, HTableData * data);
void printHTableNode(HTableNode * node);
void printHTableNodeData(HTableData * data);
void printHTableNodeName(NameType * name);







HTableData * findInHTable(HTableMap * htableMap, NameType * name);
void removeFromHTable(HTableMap * htableMap, NameType * name);
void readHTableFromFile(HTableMap * htableMap, const char * fileName);
void saveHTable(HTableMap * htableMap, const char * fileName);

#endif /* __HTABLE_H__ */
