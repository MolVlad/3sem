#include"libs.h"
#include"configure.h"
#include"my_string.h"

String * createString()
{
	#ifdef DEBUG_STRING
	printf("createString\n");
	#endif /* DEBUG_STRING */

	String * ret = (String *)calloc(1, sizeof(String));
	assert(ret);

	ret->data = (Data *)calloc(INIT_STRING_SIZE + 1, sizeof(Data));
	assert(ret->data);

	ret->maxSize = INIT_STRING_SIZE;

	#ifdef DEBUG_STRING
	printf("Create String, max size = %d\n", ret->maxSize);
	#endif /* DEBUG_STRING */

	return ret;
}

void deleteString(String * string)
{
	assert(string);

	#ifdef DEBUG_STRING
	printf("Delete String with current size: %d\n", string->currentSize);
	#endif /* DEBUG_STRING */

	free(string->data);
	free(string);
	string = NULL;

	#ifdef DEBUG_STRING
	printf("deleteString successful\n");
	#endif /* DEBUG_STRING */
}

void clearString(String * string)
{
	assert(string);

	#ifdef DEBUG_STRING
	printf("Clear String with current size = %d\n", string->currentSize);
	#endif /* DEBUG_STRING */

	if(string->maxSize > TRESHOLD_STRING_SIZE)
	{
		string->data = (Data *)realloc(string->data, (AFTER_TRESHOLD_STRING_SIZE + 1) * sizeof(Data));
		assert(string->data);

		#ifdef DEBUG_STRING
		printf("clearString's realloc: %d -> %d\n", string->maxSize, AFTER_TRESHOLD_STRING_SIZE);
		#endif /* DEBUG_STRING */

		string->maxSize = AFTER_TRESHOLD_STRING_SIZE;
	}

	string->currentSize = 0;
	memset(&string->data[0], '\0', sizeof(Data) * (string->maxSize + 1));

	#ifdef DEBUG_STRING
	printf("clearString successful\n");
	#endif /* DEBUG_STRING */
}

// Separate function in case of change of data type
void copyStringElement(Data * destination, Data * element)
{
	assert(destination);
	assert(element);

	destination[0] = *element;
	destination[1] = '\0';
}

void putInString(String * string, Data * data)
{
	assert(string);
	assert(data);

	if(string->currentSize == string->maxSize)
	{
		string->data = (Data *)realloc(string->data, ((string->maxSize * 2) + 1) * sizeof(Data));
		assert(string->data);

		#ifdef DEBUG_STRING
		printf("putInString's realloc: %d -> %d\n", string->maxSize, string->maxSize * 2);
		#endif /* DEBUG_STRING */

		string->maxSize *= 2;
	}

	copyStringElement(&string->data[string->currentSize], data);
	string->currentSize++;
}

String * copyString(String * original)
{
	#ifdef DEBUG_STRING
	printf("copyString\n");
	#endif /* DEBUG_STRING */

	assert(original);

	String * ret = (String *)calloc(1, sizeof(String));
	assert(ret);

	ret->data = (Data *)calloc(original->maxSize, sizeof(Data));
	assert(ret->data);

	ret->maxSize = original->maxSize;
	ret->currentSize = original->currentSize;

	int i;
	for(i = 0; i < original->currentSize; i++)
		copyStringElement(&ret->data[i], &original->data[i]);

	#ifdef DEBUG_STRING
	printf("copyString successful\n");
	#endif /* DEBUG_STRING */

	return ret;
}

Flag areStringSame(String * first, String * second)
{
	#ifdef DEBUG_STRING
	printf("areStringSame\n");
	#endif /* DEBUG_STRING */

	Flag ret = FALSE;
	if(strcmp(first->data, second->data) == 0)
		ret = TRUE;

	#ifdef DEBUG_STRING
	printf("areStringSame: %d\n", ret);
	#endif /* DEBUG_STRING */

	return ret;
}

int scanStringFromStream(int stream, String * string)
{
	#ifdef DEBUG_STRING
	printf("scanString\tstream = %d\n", stream);
	#endif /* DEBUG_STRING */

	assert(string);

	clearString(string);

	Flag isAll = FALSE;
	Data c;
	int n;
	do
	{
		n = read(stream, &c, 1);
		if(n == -1)
			return -1;
		else if(c == EOF)
			return -2;
		else if(c == '\n')
			isAll = TRUE;
		else
			putInString(string, &c);
	}
	while(isAll == FALSE);

	#ifdef DEBUG_STRING
	printf("Scanned String from file with current size = %d\n", string->currentSize);
	#endif /* DEBUG_STRING */

	return string->currentSize;
}

Flag scanStringFromFile(FILE * file, String * string)
{
	#ifdef DEBUG_STRING
	printf("scanStringFromFile\n");
	#endif /* DEBUG_STRING */

	assert(string);
	assert(file);

	Flag isAll = FALSE;
	Flag isEOF= FALSE;
	Data c;

	clearString(string);

	do
	{
		c = fgetc(file);
		if(c == '\n')
			isAll = TRUE;
		else if(c == EOF)
		{
			isAll = TRUE;
			isEOF = TRUE;
		}
		else
			putInString(string, &c);
	}
	while(isAll == FALSE);

	#ifdef DEBUG_STRING
	printf("Scanned String from file with current size = %d\n", string->currentSize);
	if(isEOF == TRUE)
		printf("scanStringFromFile found EOF");
	#endif /* DEBUG_STRING */

	return isEOF;
}

void printStringToStream(int stream, String * string)
{
	assert(string);

	#ifdef DEBUG_STRING
	printf("Print String with current size = %d\n", string->currentSize);
	#endif /* DEBUG_STRING */

	write(stream, string->data, string->currentSize);
	write(stream, "\n", 1);
}

void printStringToFile(FILE * file, String * string)
{
	assert(file);
	assert(string);

	#ifdef DEBUG_STRING
	printf("Print String to file with current size = %d\n", string->currentSize);
	#endif /* DEBUG_STRING */

	fprintf(file, "%s\n", string->data);
}

