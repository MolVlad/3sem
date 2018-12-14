#include"libs.h"
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

	BTreeNode * compared = btreeMap->root;
	if(compared == NULL)
	{
		btreeMap->root = node;
		return 0;
	}

	BTreeNode * parent;
	int comparison;
	while(compared != NULL)
	{
		parent = compared;
		comparison = stringCompare(compared->data->login, node->data->login);
		if(comparison > 0)
			compared = compared->right;
		else if(comparison < 0)
			compared = compared->left;
		else
			return -1;
	}

	if(comparison > 0)
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

void saveBTree(BTreeMap * btreeMap, const char * fileName)
{
	assert(btreeMap);
	assert(fileName);

	remove(FILE_LIST);

	int fd = creat(FILE_LIST, O_WRONLY);
	CHECK("open", fd);

	saveBTreeNode(btreeMap->root, fd);

	printf("BTree saved\n");

	close(fd);
}

void saveBTreeNode(BTreeNode * node, int fd)
{
	if(node == NULL)
		return;

	saveBTreeNode(node->right, fd);
	saveBTreeData(node->data, fd);
	saveBTreeNode(node->left, fd);
}

void printBTreeNode(BTreeNode * node)
{
	if(node == NULL)
		return;

	printBTreeNode(node->right);
	printBTreeData(node->data);
	printBTreeNode(node->left);
}

void saveBTreeData(BTreeData * data, int fd)
{
	assert(data);

	printStringToStream(fd, data->login);
	write(fd, "\n", 1);
}

void printBTreeData(BTreeData * data)
{
	assert(data);

	printStringToStream(STDOUT, data->login);
	printf("\n");
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

BTreeData * convertToBTreeData(String * login, int pid)
{
	assert(login);

	BTreeData * ret = (BTreeData *)calloc(1, sizeof(BTreeData));
	assert(ret);

	ret->login = copyString(login);
	assert(ret->login);

	ret->pid = pid;

	return ret;
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

	int comparison = stringCompare(compared->data->login, login);
	if(comparison == 0)
		return compared->data;
	else if(comparison > 0)
		return findBTreeNode(compared->right, login);
	else
		return findBTreeNode(compared->left, login);

	return NULL;
}

void deleteFromBTree(BTreeMap * btreeMap, String * target)
{
	assert(btreeMap);
	assert(target);

	btreeMap->root = deleteFromBTreeNode(btreeMap->root, target);
}

BTreeNode * deleteFromBTreeNode(BTreeNode * node, String * target)
{
	assert(target);

	if(node == NULL)
		return NULL;

	assert(node->data);
	assert(node->data->login);

	int comparison = stringCompare(node->data->login, target);
	if(comparison > 0)
	{
		node->right = deleteFromBTreeNode(node->right, target);
		return node;
	}
	else if(comparison < 0)
	{
		node->left = deleteFromBTreeNode(node->left, target);
		return node;
	}

	BTreeNode * ret;
	if(node->right == NULL)
		ret = node->left;
	else if(node->left == NULL)
		ret = node->right;
	else
	{
		ret = node->right;
		BTreeNode * pointer = ret;
		BTreeNode * parent;
		while(pointer != NULL)
		{
			parent = pointer;
			pointer = pointer->left;
		}

		parent->left = node->left;
	}

	node->right = NULL;
	node->left = NULL;
	deleteBTreeNode(node);

	return ret;
}
