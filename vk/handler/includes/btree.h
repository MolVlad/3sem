#ifndef __BTREE_H__
#define __BTREE_H__

typedef struct
{
	int pid;
	char * ip;
	String * login;
} BTreeData;

typedef struct BTreeNode
{
	struct BTreeNode * left;
	struct BTreeNode * right;
	BTreeData * data;
} BTreeNode;

typedef struct
{
	BTreeNode * root;
} BTreeMap;

extern BTreeMap * btreeMap;

// Function for use
BTreeMap * createBTree();
void deleteBTree(BTreeMap * btreeMap);
void insertToBTree(BTreeMap * btreeMap, BTreeData * data);
void printBTree(BTreeMap * btreeMap);
BTreeData * findInBTree(BTreeMap * btreeMap, String * login);
void deleteFromBTree(BTreeMap * btreeMap, String * string);

// Private function
void deleteBTreeNode(BTreeNode * node);
void deleteBTreeData(BTreeData * data);
BTreeData * convertToBTreeData(String * login, int pid);
BTreeNode * createBTreeNode(BTreeData * data);
void printBTreeNode(BTreeNode * node);
void printBTreeData(BTreeData * data);
BTreeData * findBTreeNode(BTreeNode * compared, String * login);
int putInBTree(BTreeMap * btreeMap, BTreeNode * node);

#endif /* __BTREE_H__ */
