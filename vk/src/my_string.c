#include"libs.h"
#include"my_string.h"
#include"configure.h"

String * createString()
{
	String * ret = (String *)calloc(1, sizeof(String));
	ret->data = (Data *)calloc(INIT_STRING_SIZE + 1, sizeof(Data));
	ret->maxSize = INIT_STRING_SIZE;

	#ifdef DEBUG_STRING
	printf("Create String, max size = %d\n", ret->maxSize);
	#endif /* DEBUG_STRING */

	return ret;
}

void deleteString(String * string)
{

	#ifdef DEBUG_STRING
	printf("Delete String with current size: %d\n", string->currentSize);
	#endif /* DEBUG_STRING */

	free(string->data);
	free(string);
}

void clearString(String * string)
{
	#ifdef DEBUG_STRING
	printf("Clear String with current size = %d\n", string->currentSize);
	#endif /* DEBUG_STRING */

	if(string->maxSize > TRESHOLD_STRING_SIZE)
	{
		string->data =  (Data *)realloc(string->data, (AFTER_TRESHOLD_STRING_SIZE + 1) * sizeof(Data));

		#ifdef DEBUG_STRING
		if(!string->data)
		{
			perror("realloc");
			exit(1);
		}
		printf("clearString's realloc: %d -> %d\n", string->maxSize, AFTER_TRESHOLD_STRING_SIZE);
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
		printf("putInString's realloc: %d -> %d\n", string->maxSize, string->maxSize * 2);
		#endif /* DEBUG_STRING */

		string->maxSize *= 2;
	}

	copyData(&string->data[string->currentSize], data);
	string->currentSize++;
}

int scanString(String * string)
{
	Flag isAll = FALSE;
	Data c;

	clearString(string);

	do
	{
		c = getchar();
		if(c == '\n')
			isAll = TRUE;
		else
			putInString(string, &c);
	}
	while(isAll == FALSE);

	#ifdef DEBUG_STRING
	printf("Scan String with current size = %d\n", string->currentSize);
	#endif /* DEBUG_STRING */

	return string->currentSize;
}

void printString(String * string)
{
	#ifdef DEBUG_STRING
	printf("Print String with current size = %d\n", string->currentSize);
	#endif /* DEBUG_STRING */

	write(OUTPUT_STREAM, string->data, string->currentSize);
	printf("\n");
}
