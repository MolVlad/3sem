#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#define MAX_SIZE_WORD 100
#define MAX_SIZE_TEXT 10000
#define MAX_SIZE_DELIMITERS 10
#define MAX_NUM_WORDS 20

char * Allocate_memory(int num_symbols);
char ** Allocate_memory_array(int array_size, int word_size);
int Print_words(char ** words, int count);
int Split(char * str, const char * delimiters, char ** words, int * count);

int main()
{
	char * text = Allocate_memory(MAX_SIZE_TEXT);
	printf("Print text:\n");
	fgets(text, MAX_SIZE_TEXT, stdin);

	char * delimiters = Allocate_memory(MAX_SIZE_DELIMITERS);
	printf("Print delimiters:\n");
	fgets(delimiters, MAX_SIZE_DELIMITERS, stdin);

	char ** words = Allocate_memory_array(MAX_NUM_WORDS, MAX_SIZE_WORD);
	int count;

	assert(!Split(text, delimiters, words, &count));
//	assert(!Print_words(words, count));

	return 0;
}

char * Allocate_memory(int num_symbols)
{
	char * ret = (char *)malloc((num_symbols + 1) * sizeof(char));
	assert(ret);

	return ret;
}

char ** Allocate_memory_array(int array_size, int word_size)
{
	int i;

	char ** ret = (char **)malloc(array_size * sizeof(char *));
	assert(ret);

	for(i = 0; i < array_size; i++)
	{
		ret[i] = (char *)malloc((word_size + 1) * sizeof(char));
		assert(ret[i]);
	}

	return ret;
}

int Print_words(char ** words, int count)
{
	if(!words)
		return 1;
	int i;


	for(i = 0; i < count; i++)
	{
		if(!words[i])
			return 1;
		printf("%s\n", words[i]);
	}

	return 0;
}

int Split(char * str, const char * delimiters, char ** words, int * count)
{
	char * fragment = Allocate_memory(MAX_SIZE_WORD);

	int i = 0;
	fragment = strtok(str, delimiters);
	while(fragment)
	{
		printf("%s\n", fragment);
		strcpy(words[i], fragment);
		i++;
		fragment = strtok(NULL, delimiters);
	}

	*count = i;

	free(fragment);

	return 0;
}

void Free_memory_array(char ** array, int array_size)
{
	int i;

	for(i = 0; i < array_size; i++)
		free(array[i]);

	free(array);
}
