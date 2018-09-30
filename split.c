/*
  ******************************************************************************
  * @file		split.c
  * @author		Molodcov Vladislav
  * @institute		MIPT
  * @departament	DREC
  * @group		717
  * @version		V1.1
  * @date		19-September-2018
  * @brief		SPLIT-functions
  ******************************************************************************
*/

/* Includes ----------------------------------------------------------------------*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

/* Defgroup for sizes ------------------------------------------------------------*/

#define MAX_SIZE_TEXT		10000
#define MAX_SIZE_DELIMITERS	10
#define MAX_NUM_WORDS		100
#define MAX_SIZE_WORD		100

/* Structure for SPLIT -----------------------------------------------------------*/

typedef struct
{
	int maxSizeText;
	int maxSizeDelimiters;
	int maxNumWords;
	int maxSizeWord;
	int count;

	char * text;
	char * delimiters;
	char ** words;
} splitStruct;

/* Declarations of functions for memory ------------------------------------------*/

// Вы не используете эти три ф-и в main'е, так что можно обойтись и без их объявления совсем.
// Оставить только определение после main'a.

char ** allocateMemoryArray(int array_size, int word_size);
char * allocateMemory(int num_symbols);
void freeMemoryArray(char ** array, int array_size);

/* Declarations of SPLIT-functions -----------------------------------------------*/

int splitInit(splitStruct * split);
int splitScan(splitStruct * split);
int splitSplit(splitStruct * split);
int splitPrint(splitStruct * split);
int splitFree(splitStruct * split);

/* Main function -----------------------------------------------------------------*/

int main()
{
	splitStruct split;
	split.maxSizeText = 		MAX_SIZE_TEXT;
	split.maxSizeDelimiters = 	MAX_SIZE_DELIMITERS;
	split.maxNumWords = 		MAX_NUM_WORDS;
	split.maxSizeWord = 		MAX_SIZE_WORD;

	if(splitInit(&split))
	{
		printf("Init error\n");
		return 1;
	}

	if(splitScan(&split))
	{
		printf("Scan error\n");
		return 2;
	}

	if(splitPrint(&split))
	{
		printf("Print error\n");
		return 3;
	}

	if(splitFree(&split))
	{
		printf("Free error\n");
		return 4;
	}

	return 0;
}

/* Prototypes of functions for memory---------------------------------------------*/

char * allocateMemory(int numSymbols)
{
	char * ret = (char *)malloc((numSymbols + 1) * sizeof(char));

	return ret;
}

char ** allocateMemoryArray(int arraySize, int wordSize)
{
	char ** ret = (char **)malloc(arraySize * sizeof(char *));

	int i;

	for(i = 0; i < arraySize; i++)
		ret[i] = allocateMemory(wordSize);

	return ret;
}

void freeMemoryArray(char ** array, int arraySize)
{
	int i;

	for(i = 0; i < arraySize; i++)
		free(array[i]);

	free(array);
}

/* Prototypes of SPLIT-functions -------------------------------------------------*/

int splitInit(splitStruct * split)
{
	split->text =		allocateMemory(split->maxSizeText);
	split->delimiters =	allocateMemory(split->maxSizeDelimiters);
	split->words =		allocateMemoryArray(split->maxNumWords, split->maxSizeWord);
	split->count =		0;

	if(!split->text || !split->delimiters || !split->words)
		return 1;

	int i;

	for(i = 0; i < split->maxNumWords; i++)
	{
		if(!split->words[i])
			return 1;
	}

	return 0;
}

int splitScan(splitStruct * split)
{
	printf("Print text:\n");
	fgets(split->text, split->maxSizeText, stdin);

	printf("Print delimiters:\n");
	fgets(split->delimiters, split->maxSizeDelimiters, stdin);

	return 0;
}

int splitSplit(splitStruct * split)
{
	char * fragment = allocateMemory(MAX_SIZE_WORD);

	int i = 0;

	fragment = strtok(split->text, split->delimiters);
	while(fragment)
	{
		if(!split->words[i])
			return 1;

		strcpy(split->words[i], fragment);
		fragment = strtok(NULL, split->delimiters);
		i++;
	}

	split->count = i;

	free(fragment);

	return 0;
}

int splitPrint(splitStruct * split)
{
	if(!split->words)
		return 1;

	int i;
	int n = split->count;
	for(i = 0; i < n; i++)
	{
		if(!split->words[i])
			return 1;

		printf("%s\n", split->words[i]);
	}

	return 0;
}

int splitFree(splitStruct * split)
{
	free(split->text);
	free(split->delimiters);
	freeMemoryArray(split->words, split->maxNumWords);

	return 0;
}
