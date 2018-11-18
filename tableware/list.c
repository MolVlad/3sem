#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

typedef struct Node
{
	int number;
	struct Node * next;
	struct Node * prev;

	char * name;
	int timeToWash;
	int timeToWipe;
} Node;

typedef struct List
{
	int size;
	struct Node * first;
	struct Node * last;
} List;

List * createList();
int deleteList(List * list);
void addToList(List * list, const char * name, int timeToWash, int timeToWipe);
void printList(List * list);

Node * createNode(const char * name, int timeToWash, int timeToWipe);
int deleteNode(Node * node);

int main()
{
	List * list = createList();

	addToList(list, "lol", 1, 2);
	addToList(list, "kek", 3, 4);
	addToList(list, "azaz", 5, 10);

	printList(list);

	deleteList(list);
}

List * createList()
{
	return (List *)calloc(1, sizeof(List));
}

int deleteList(List * list)
{
	if(list == NULL)
		return 0;

	deleteNode(list->first);
	free(list);
	return 0;
}

void addToList(List * list, const char * name, int timeToWash, int timeToWipe)
{
	Node * new = createNode(name, timeToWash, timeToWipe);
	new->next = NULL;

	if(list->first == NULL)
	{
		list->first = new;
		list->last = new;
		new->number = 1;
	}
	else
	{
		new->prev = list->last;
		list->last->next = new;
		new->number = list->last->number + 1;
		list->last = new;
	}
}

void printList(List * list)
{
	Node * current;
	for(current = list->first; current != NULL; current = current->next)
	{
		printf("%d) %s %d %d\n", current->number, current->name, current->timeToWash, current->timeToWipe);
	}
}

Node * createNode(const char * name, int timeToWash, int timeToWipe)
{
	Node * current = (Node *)calloc(1, sizeof(Node));

	current->name = (char *)calloc(strlen(name) + 1, sizeof(char));
	strcpy(current->name, name);
	current->timeToWash = timeToWash;
	current->timeToWipe = timeToWipe;

	return current;
}

int deleteNode(Node * node)
{
	if(node == NULL)
		return 0;

	deleteNode(node->next);
	free(node->name);
	free(node);
	return 0;
}
