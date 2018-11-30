#include"libs.h"

#define INIT_STRING_SIZE 10
#define TRESHOLD_STRING_SIZE 80
#define AFTER_TRESHOLD_STRING_SIZE 40
#define OUTPUT_STREAM 1

#define DEBUG_STRING 1

typedef char Flag;
typedef char Data;
typedef struct
{
	Data * data;

	int currentSize;
	int maxSize;
} String;

String * createString()
{
	String * ret = (String *)calloc(1, sizeof(String));
	ret->data = (Data *)calloc(INIT_STRING_SIZE + 1, sizeof(Data));
	ret->maxSize = INIT_STRING_SIZE;

	return ret;
}

void deleteString(String * string)
{

	#ifdef DEBUG_STRING
	printf("deleteString size: %d\n", string->currentSize);
	#endif /* DEBUG_STRING */

	free(string->data);
	free(string);
}

void clearString(String * string)
{
	if(string->maxSize > TRESHOLD_STRING_SIZE)
	{
		string->data =  (Data *)realloc(string->data, (AFTER_TRESHOLD_STRING_SIZE + 1) * sizeof(Data));

		#ifdef DEBUG_STRING
		if(!string->data)
		{
			perror("realloc");
			exit(1);
		}
		printf("clearString realloc: %d -> %d\n", string->maxSize, AFTER_TRESHOLD_STRING_SIZE);
		#endif /* DEBUG_STRING */

		string->maxSize = AFTER_TRESHOLD_STRING_SIZE;
	}

	string->currentSize = 0;
	string->data[0] = '\0';
}

// Separate function in case of change of data type
void copyData(Data * destination, Data * data)
{
	destination[0] = *data;
	destination[1] = '\0';
}

void putInString(String * string, Data * data)
{
	if(string->currentSize == string->maxSize)
	{
		string->data = (Data *)realloc(string->data, ((string->maxSize * 2) + 1) * sizeof(Data));

		#ifdef DEBUG_STRING
		if(!string->data)
		{
			perror("realloc");
			exit(1);
		}
		printf("putInString realloc: %d -> %d\n", string->maxSize, string->maxSize * 2);
		#endif /* DEBUG_STRING */

		string->maxSize *= 2;
	}

	copyData(&string->data[string->currentSize], data);
	string->currentSize++;
}

int scanString(String * string)
{
	Flag isAll = 0;
	Data c;

	do
	{
		c = getchar();
		if(c == '\n')
			isAll = 1;
		else
			putInString(string, &c);
	}
	while(!isAll);

	return string->currentSize;
}

void printString(String * string)
{
	write(OUTPUT_STREAM, string->data, string->currentSize);
	printf("\n");
}

int main()
{
	String * string = createString();

	scanString(string);
	printString(string);

	clearString(string);
	scanString(string);
	printString(string);

	deleteString(string);

	return 0;
}
