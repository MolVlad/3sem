#ifndef __LIST_H__
#define __LIST_H__

/* Customized types for data------------------------------*/

// You can change this part for your aims

typedef struct Data
{
	char * name;
	int timeToWash;
	int timeToWipe;
} Data;
typedef const char * TypeOfDesired;

/* Customized functions for data---------------------------*/

// You can change this functions for your aims

Data * createData(const char * name, int timeToWash, int timeToWipe);
int deleteData(Data * data);
void printData(Data * data);
int isDesired(Data * data, TypeOfDesired desired);
Data * copyData(Data * data);

/* List implementation-------------------------------------*/

// You shouldn't change this part

typedef struct Node
{
	int number;
	struct Node * next;

	Data * data;
} Node;

typedef struct List
{
	int size;
	char * name;
	Node * first;
	Node * last;
} List;

List * createList(char * name);
int deleteList(List * list);
void addToList(List * list, Data * data);
void printList(List * list);
Data * findInList(List * list, TypeOfDesired desired);

Node * createNode(Data * data);
int deleteNode(Node * node);
Node * copyNode(Node * node);

#endif /* __LIST_H__ */
