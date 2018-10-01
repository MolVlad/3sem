/*
  ******************************************************************************
  * @file		split.c
  * @author		Molodcov Vladislav
  * @institute		MIPT
  * @departament	DREC
  * @group		717
  * @version		V1.1
  * @date		1-October-2018
  * @brief		split-functions
  ******************************************************************************
*/

#include "split.h"

/* Declarations of functions for memory ------------------------------------------*/

char ** allocateMemoryArray(int array_size, int word_size);
char * allocateMemory(int num_symbols);
void freeMemoryArray(char ** array, int array_size);

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
	#ifdef CONSOLE
	printf("Print text:\n");
	#endif /* CONSOLE */

	scanf("%[^\n]", split->text);
	getchar();

	#ifdef SCAN_DELIMITERS
	#ifdef CONSOLE
	printf("Print delimiters:\n");
	#endif /* CONSOLE */

	scanf("%[^\n]", split->delimiters);
	getchar();
	#endif /* SCAN_DELIMITERS */

	return 0;
}

int splitSplit(splitStruct * split)
{
	char * fragment = allocateMemory(MAX_SIZE_WORD);

	int i = 0;

	fragment = strtok(split->text, split->delimiters);

	#ifdef SPLIT_WITH_DELAY
	if(!fragment)
		return 1;

	split->delay = atoi(fragment);

	fragment = strtok(NULL, split->delimiters);
	#endif /* SPLIT_WITH_DELAY */

	while(fragment)
	{
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
