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
HTableData * findInHTable(HTableMap * htableMap, NameType * name);

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
void saveHTableNode(FILE * file, HTableNode * node);
void saveHTableNodeName(FILE * file, NameType * name);
void saveHTableNodeData(FILE * file, HTableData * data);
HTableData * findHTableNodeInChein(HTableNode * compared, NameType * name);
Flag areNamesSame(NameType * first, NameType * second);





void readHTableFromFile(HTableMap * htableMap, const char * fileName);

#endif /* __HTABLE_H__ */
