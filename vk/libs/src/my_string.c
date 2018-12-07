#include"libs.h"
#include"config.h"
#include"my_string.h"

String * createString()
{
	String * ret = (String *)calloc(1, sizeof(String));
	assert(ret);

	ret->data = (Data *)calloc(INIT_STRING_SIZE + 1, sizeof(Data));
	assert(ret->data);

	ret->maxSize = INIT_STRING_SIZE;

	return ret;
}

void deleteString(String * string)
{
	assert(string);

	free(string->data);
	free(string);
	string = NULL;
}

void clearString(String * string)
{
	assert(string);

	if(string->maxSize > TRESHOLD_STRING_SIZE)
	{
		string->data = (Data *)realloc(string->data, (AFTER_TRESHOLD_STRING_SIZE + 1) * sizeof(Data));
		assert(string->data);

		string->maxSize = AFTER_TRESHOLD_STRING_SIZE;
	}

	string->currentSize = 0;
	memset(&string->data[0], '\0', sizeof(Data) * (string->maxSize + 1));
}

void putInString(String * string, Data * data)
{
	assert(string);
	assert(data);

	if(string->currentSize == string->maxSize)
	{
		string->data = (Data *)realloc(string->data, ((string->maxSize * 2) + 1) * sizeof(Data));
		assert(string->data);

		string->maxSize *= 2;
	}

	string->data[string->currentSize] = *data;
	string->currentSize++;
}

String * copyString(String * original)
{
	assert(original);

	String * ret = (String *)calloc(1, sizeof(String));
	assert(ret);

	ret->data = (Data *)calloc(original->maxSize, sizeof(Data));
	assert(ret->data);

	ret->maxSize = original->maxSize;
	ret->currentSize = original->currentSize;

	int i;
	for(i = 0; i < original->currentSize; i++)
		ret->data[i] = original->data[i];

	return ret;
}

int stringCompare(String * first, String * second)
{
	return strcmp(first->data, second->data);
}

int scanStringFromStream(int fd, String * string, int maxNum)
{
	assert(string);

	Flag isLimitedNumber;
	if(maxNum == 0)
		return 0;
	if(maxNum < 0)
		isLimitedNumber = FALSE;
	else
		isLimitedNumber = TRUE;

	clearString(string);

	Flag isAll = FALSE;
	Data c;
	while(isAll == FALSE)
	{
		if(read(fd, &c, sizeof(Data)) == 0)
			return -1;

		if(c == '\n')
			isAll = TRUE;
		else
			putInString(string, &c);

		if(isLimitedNumber == TRUE)
		{
			maxNum--;
			if(maxNum == 0)
				isAll = TRUE;
		}
	}

	return string->maxSize;
}

int printStringToStream(int stream, String * string)
{
	assert(string);

	int n = write(stream, string->data, string->currentSize);
	if(n != string->currentSize)
		return -1;

	return 0;
}
