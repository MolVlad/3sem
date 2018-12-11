#include"libs.h"
#include"config.h"
#include"general_config.h"
#include"my_string.h"
#include"btree.h"

BTreeMap * createBTree()
{
	return (BTreeMap *)calloc(1, sizeof(BTreeMap));
}

void insertToBTree(BTreeMap * btreeMap, BTreeData * data)
{
	assert(btreeMap);
	assert(data);

	BTreeNode * node = createBTreeNode(data);
	assert(node);
	putInBTree(btreeMap, node);
}

int putInBTree(BTreeMap * btreeMap, BTreeNode * node)
{
	assert(btreeMap);
	assert(node);

	BTreeNode * pointer = btreeMap->root;
	if(pointer == NULL)
	{
		btreeMap->root = node;
		return 0;
	}

	BTreeNode * parent;
	while(pointer != NULL)
	{
		parent = pointer;
		if(stringCompare(pointer->data->login, node->data->login) > 0)
			pointer = pointer->right;
		else
			pointer = pointer->left;
	}

	if(parent->data->pid > parent->data->pid)
		parent->right = node;
	else
		parent->left = node;

	return 0;
}

void printBTree(BTreeMap * btreeMap)
{
	assert(btreeMap);

	printBTreeNode(btreeMap->root);
}

void printBTreeNode(BTreeNode * node)
{
	if(node == NULL)
		return;

	printBTreeNode(node->left);
	printBTreeNode(node->right);

	printBTreeData(node->data);
}

void deleteBTree(BTreeMap * btreeMap)
{
	assert(btreeMap);

	deleteBTreeNode(btreeMap->root);
	free(btreeMap);
}

void deleteBTreeNode(BTreeNode * node)
{
	if(node == NULL)
		return;

	deleteBTreeNode(node->left);
	deleteBTreeNode(node->right);
	deleteBTreeData(node->data);
	free(node);
}

void deleteBTreeData(BTreeData * data)
{
	assert(data);

	deleteString(data->login);
	free(data->ip);
	free(data);
}

BTreeNode * createBTreeNode(BTreeData * data)
{
	assert(data);

	BTreeNode * ret = (BTreeNode *)calloc(1, sizeof(BTreeNode));
	assert(ret);

	ret->data = data;

	return ret;
}

BTreeData * convertToBTreeData(String * login, char * ip, int pid)
{
	assert(login);
	assert(ip);

	BTreeData * ret = (BTreeData *)calloc(1, sizeof(BTreeData));
	assert(ret);

	ret->login = copyString(login);
	assert(ret->login);

	ret->ip = (char *)calloc(strlen(ip) + 1, sizeof(char));
	assert(ret->ip);
	strcpy(ret->ip , ip);

	ret->pid = pid;

	return ret;
}

void printBTreeData(BTreeData * data)
{
	assert(data);

	printf("login = ");
	printStringToStream(STDOUT, data->login);
	printf("pid = %d\n", data->pid);
	printf("ip = %s\n", data->ip);
}

BTreeData * findInBTree(BTreeMap * btreeMap, String * login)
{
	assert(login);

	BTreeData * ret = findBTreeNode(btreeMap->root, login);

	return ret;
}

BTreeData * findBTreeNode(BTreeNode * compared, String * login)
{
	assert(login);

	if(compared == NULL)
		return NULL;

	if(stringCompare(compared->data->login, login) == 0)
		return compared->data;

	if(compared->left != NULL)
		return findBTreeNode(compared->left, login);

	if(compared->right != NULL)
		return findBTreeNode(compared->right, login);

	return NULL;
}
