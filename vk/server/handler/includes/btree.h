#ifndef __BTREE_H__
#define __BTREE_H__

typedef struct
{
	int pid;
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
void saveBTree(BTreeMap * btreeMap, const char * fileName);
BTreeData * findInBTree(BTreeMap * btreeMap, String * login);
void deleteFromBTree(BTreeMap * btreeMap, String * string);

// Private function
void deleteBTreeNode(BTreeNode * node);
void deleteBTreeData(BTreeData * data);
BTreeData * convertToBTreeData(String * login, int pid);
BTreeNode * createBTreeNode(BTreeData * data);
void printBTreeNode(BTreeNode * node);
void printBTreeData(BTreeData * data);
void saveBTreeNode(BTreeNode * node, int fd);
void saveBTreeData(BTreeData * data, int fd);
BTreeData * findBTreeNode(BTreeNode * compared, String * login);
int putInBTree(BTreeMap * btreeMap, BTreeNode * node);
BTreeNode * deleteFromBTreeNode(BTreeNode * node, String * target);

#endif /* __BTREE_H__ */
