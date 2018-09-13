#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#define MAX_SIZE_WORD 100
#define MAX_SIZE_TEXT 10000
#define MAX_SIZE_DELIMITERS 10
#define MAX_NUM_WORDS 100

int Split(const char * str, const char * delimiters, char *** words, int * count);
void Print_words(char ** words, int count);

int main()
{
/* Scan text------------------------------------------------------------*/

	char * text = malloc((MAX_SIZE_TEXT + 1) * sizeof(char));
	assert(text);
	printf("Print text:\n");
	scanf("%s", text);
	if(strlen(text) > MAX_SIZE_TEXT)
		assert(!"text_overflow");

/* Scan delimiters------------------------------------------------------*/

	char * delimiters = malloc((MAX_SIZE_DELIMITERS + 1) * sizeof(char));
	assert(delimiters);
	printf("Print delimiters:\n");
	scanf("%s", delimiters);
	if(strlen(delimiters) > MAX_SIZE_DELIMITERS)
		assert(!"delimiters_overflow");

/* Split words----------------------------------------------------------*/

	char ** words = malloc(MAX_NUM_WORDS * sizeof(char));		//allocate memory for pointer to a word array

	int i;
	for(i = 0; i < MAX_NUM_WORDS; i++)
		words[i] = malloc((MAX_SIZE_WORD + 1) * sizeof(char));	//allocate memory for words

	int count;

	Split(text, delimiters, &words, &count);

	Print_words(words, count);

	return 0;
}

int Split(const char * str, const char * delimiters, char *** words, int * count)
{
	*words[0] = "lol";
	*words[1] = "sg";
	*count = 2;

}

void Print_words(char ** words, int count)
{
	int i;

	for(i = 0; i < count; i++)
		printf("%s\n", words[i]);
}
