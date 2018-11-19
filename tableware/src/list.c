#include"libs.h"
#include"list.h"

/* Customized functions for data---------------------------*/

// You can change this functions for your aims

int deleteData(Data * data)
{
	if(data == NULL)
		return 0;

	free(data->name);
	free(data);
	return 0;
}

Data * copyData(Data * data)
{
	Data * new = (Data *)calloc(1, sizeof(Data));

	new->name = (char *)calloc(strlen(data->name) + 1, sizeof(char));
	strcpy(new->name, data->name);
	new->timeToWash = data->timeToWash;
	new->timeToWipe = data->timeToWipe;

	return new;
}

int isDesired(Data * data, TypeOfDesired desired)
{
	return !strcmp(data->name, desired);
}

void printData(Data * data)
{
	printf("%s %d %d\n", data->name, data->timeToWash, data->timeToWipe);
}

Data * createData(const char * name, int timeToWash, int timeToWipe)
{
	Data * new = (Data *)calloc(1, sizeof(Data));

	new->name = (char *)calloc(strlen(name) + 1, sizeof(char));
	strcpy(new->name, name);
	new->timeToWash = timeToWash;
	new->timeToWipe = timeToWipe;

	return new;
}

/* List implementation-------------------------------------*/

// You shouldn't change this part

List * createList(char * name)
{
	List * list = (List *)calloc(1, sizeof(List));
	list->name = calloc(strlen(name) + 1, sizeof(char));
	strcpy(list->name, name);

	return list;
}

int deleteList(List * list)
{
	if(list == NULL)
		return 0;

	deleteNode(list->first);
	free(list->name);
	free(list);
	return 0;
}

void addToList(List * list, Data * data)
{
	Node * new = createNode(data);
	new->next = NULL;

	if(list->first == NULL)
	{
		list->first = new;
		list->last = new;
		new->number = 1;
	}
	else
	{
		list->last->next = new;
		new->number = list->last->number + 1;
		list->last = new;
	}

	list->size++;
}

void printList(List * list)
{
	printf("\nList name: %s\n", list->name);

	Node * current;
	for(current = list->first; current != NULL; current = current->next)
	{
		printf("%d) ", current->number);
		printData(current->data);
	}
}

Data * findInList(List * list, TypeOfDesired desired)
{
	Node * current;
	for(current = list->first; current != NULL; current = current->next)
		if(isDesired(current->data, desired))
			return current->data;

	return NULL;
}

Node * createNode(Data * data)
{
	Node * current = (Node *)calloc(1, sizeof(Node));

	current->data = data;

	return current;
}

int deleteNode(Node * node)
{
	if(node == NULL)
		return 0;

	deleteNode(node->next);
	deleteData(node->data);
	free(node);
	return 0;
}
