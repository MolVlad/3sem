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
	HashType hash;
	NameType * name;
	struct HTableNode * next;
	struct HTableNode * prev;

	HTableData * data;
} HTableNode;

typedef struct
{
	HTableNode ** array;
	int size;
} HTableMap;

HTableMap * createHTable();
void deleteHTable(HTableMap * htableMap);
void deleteHTableNode(HTableNode * node);
void deleteNameType(NameType * name);
void insertToHTable(HTableMap * htableMap, HTableData * data);
HTableData * findInHTable(HTableMap * htableMap, NameType * name);
void removeFromHTable(HTableMap * htableMap, NameType * name);
void readHTableFromFile(HTableMap * htableMap, const char * fileName);
void saveHTable(HTableMap * htableMap, const char * fileName);

HTableMap * htableMap;

#endif /* __HTABLE_H__ */
