#include"libs.h"
#include"configure.h"
#include"my_string.h"
#include"btree.h"

BTreeMap * createBTree()
{
	#ifdef DEBUG_BTREE
	printf("createBTree\n");
	#endif /* DEBUG_BTREE */

	BTreeMap * ret = (BTreeMap *)calloc(1, sizeof(BTreeMap));
	assert(ret);

	#ifdef DEBUG_BTREE
	printf("createBTree successful\n");
	#endif /* DEBUG_BTREE */

	return ret;
}

void insertToBTree(BTreeMap * btreeMap, BTreeData * data)
{
	assert(btreeMap);
	assert(data);

	#ifdef DEBUG_BTREE
	printf("insertToBTree\n");
	#endif /* DEBUG_BTREE */

	BTreeNode * node = createBTreeNode(data);
	assert(node);
	putInBTree(btreeMap, node);

	#ifdef DEBUG_BTREE
	printf("insertToBTree successful\n");
	#endif /* DEBUG_BTREE */
}

int putInBTree(BTreeMap * btreeMap, BTreeNode * node)
{
	assert(btreeMap);
	assert(node);

	#ifdef DEBUG_BTREE
	printf("putInBTree\n");
	#endif /* DEBUG_BTREE */

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
		if(pointer->data->pid > node->data->pid)
		{
			#ifdef DEBUG_BTREE
			printf("Move to right\n");
			#endif /* DEBUG_BTREE */

			pointer = pointer->right;
		}
		else
		{
			#ifdef DEBUG_BTREE
			printf("Move to left\n");
			#endif /* DEBUG_BTREE */

			pointer = pointer->left;
		}
	}

	if(parent->data->pid > parent->data->pid)
		parent->right = node;
	else
		parent->left = node;

	#ifdef DEBUG_BTREE
	printf("putInBTree successful\n");
	#endif /* DEBUG_BTREE */

	return 0;
}

void printBTree(BTreeMap * btreeMap)
{
	assert(btreeMap);

	#ifdef DEBUG_BTREE
	printf("printBTree\n");
	#endif /* DEBUG_BTREE */

	printBTreeNode(btreeMap->root);

	#ifdef DEBUG_BTREE
	printf("printBTree successful\n");
	#endif /* DEBUG_BTREE */
}

void printBTreeNode(BTreeNode * node)
{
	if(node == NULL)
		return;

	#ifdef DEBUG_BTREE
	printf("printBTreeNode\n");
	#endif /* DEBUG_BTREE */

	printBTreeNode(node->left);
	printBTreeNode(node->right);

	printBTreeNodeData(node->data);

	#ifdef DEBUG_BTREE
	printf("printBTreeNode successful\n");
	#endif /* DEBUG_BTREE */
}

void deleteBTree(BTreeMap * btreeMap)
{
	assert(btreeMap);

	#ifdef DEBUG_BTREE
	printf("deleteBTree\n");
	#endif /* DEBUG_BTREE */

	deleteBTreeNode(btreeMap->root);
	free(btreeMap);

	#ifdef DEBUG_BTREE
	printf("deleteBTree successful\n");
	#endif /* DEBUG_BTREE */
}

void deleteBTreeNode(BTreeNode * node)
{
	if(node == NULL)
		return;

	#ifdef DEBUG_BTREE
	printf("deleteBTreeNode\n");
	#endif /* DEBUG_BTREE */

	deleteBTreeNode(node->left);
	deleteBTreeNode(node->right);
	deleteBTreeData(node->data);
	free(node);

	#ifdef DEBUG_BTREE
	printf("deleteBTreeNode successful\n");
	#endif /* DEBUG_BTREE */
}

void deleteBTreeData(BTreeData * data)
{
	assert(data);

	#ifdef DEBUG_BTREE
	printf("deleteBTreeData\n");
	#endif /* DEBUG_BTREE */

	deleteString(data->login);
	free(data->ip);
	free(data);

	#ifdef DEBUG_BTREE
	printf("deleteBTreeData successful\n");
	#endif /* DEBUG_BTREE */
}

BTreeNode * createBTreeNode(BTreeData * data)
{
	assert(data);

	#ifdef DEBUG_BTREE
	printf("createBTreeNode\n");
	#endif /* DEBUG_BTREE */

	BTreeNode * ret = (BTreeNode *)calloc(1, sizeof(BTreeNode));
	assert(ret);

	ret->data = data;

	#ifdef DEBUG_BTREE
	printf("createBTreeNode successful\n");
	#endif /* DEBUG_BTREE */

	return ret;
}

BTreeData * convertToBTreeData(String * login, char * ip, int pid)
{
	assert(login);
	assert(ip);

	#ifdef DEBUG_BTREE
	printf("convertToBTreeData\n");
	#endif /* DEBUG_BTREE */

	BTreeData * ret = (BTreeData *)calloc(1, sizeof(BTreeData));
	assert(ret);

	ret->login = copyString(login);
	assert(ret->login);

	ret->ip = (char *)calloc(strlen(ip) + 1, sizeof(char));
	assert(ret->ip);
	strcpy(ret->ip , ip);

	ret->pid = pid;

	#ifdef DEBUG_BTREE
	printf("convertToBTreeData successful\n");
	#endif /* DEBUG_BTREE */

	return ret;
}

void printBTreeNodeData(BTreeData * data)
{
	assert(data);

	printf("login = ");
	printString(data->login);
	printf("pid = %d\n", data->pid);
	printf("ip = %s\n", data->ip);
}

BTreeData * findInBTree(BTreeMap * btreeMap, String * login)
{
	assert(login);

	#ifdef DEBUG_BTREE
	printf("findInBTree\n");
	#endif /* DEBUG_BTREE */

	BTreeData * ret = findBTreeNode(btreeMap->root, login);

	#ifdef DEBUG_BTREE
	printf("findInBTree: is it found = %d\n", (ret != NULL));
	#endif /* DEBUG_BTREE */

	return ret;
}

BTreeData * findBTreeNode(BTreeNode * compared, String * login)
{
	assert(login);

	#ifdef DEBUG_BTREE
	printf("compareBTreeNodeName\n");
	#endif /* DEBUG_BTREE */

	if(compared == NULL)
		return NULL;

	if(areStringSame(compared->data->login, login) == TRUE)
		return compared->data;

	if(compared->left != NULL)
		return findBTreeNode(compared->left, login);

	if(compared->right != NULL)
		return findBTreeNode(compared->right, login);

	return NULL;
}
