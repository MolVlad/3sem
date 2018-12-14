#include"libs.h"
#include"general_config.h"
#include"my_string.h"

String * createString()
{
	String * ret = (String *)calloc(1, sizeof(String));
	assert(ret);
ret->data = (Data *)calloc(INIT_STRING_SIZE + 1, sizeof(Data)); assert(ret->data);

	ret->maxSize = INIT_STRING_SIZE;

	return ret;
}

void deleteString(String * string)
{
	if(string == NULL)
		return;

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

//maxNum < 0 means that this parameter no effect
int scanStringFromStream(int fd, String * string, int maxNum)
{
	assert(string);
	clearString(string);

	Data c;

	if(maxNum == 0)
		return 0;
	else if(maxNum < 0)
		do {
			if(read(fd, &c, sizeof(Data)) == 0)
				return -1;

			if(c != '\n')
				putInString(string, &c);
		} while(c != '\n');
	else
		while(maxNum > 0)
		{
		if(read(fd, &c, sizeof(Data)) == 0)
			return -1;

		maxNum--;
		putInString(string, &c);

		}

	return string->currentSize;
}

//maxNum < 0 means that this parameter no effect
int scanTextFromStream(int fd, String * string, int maxNum)
{
	assert(string);
	clearString(string);

	int spaceCounter = 0;
	Data c;

	if(maxNum == 0)
		return 0;
	else if(maxNum > 0)
		while(maxNum)
		{
			if(read(fd, &c, sizeof(Data)) == 0)
				return -1;

			maxNum--;
			putInString(string, &c);
		}
	else
		while(spaceCounter != NUM_SPACE_TO_BREAK)
		{
			if(read(fd, &c, sizeof(Data)) == 0)
				return string->currentSize;

			if(c == '\n')
				spaceCounter++;
			else
				spaceCounter = 0;

			putInString(string, &c);
		}

	return string->currentSize;
}

int printStringToStream(int stream, String * string)
{
	assert(string);

	int n = write(stream, string->data, string->currentSize);
	if(n != string->currentSize)
		return -1;

	return 0;
}

String * pidToString()
{
	String * ret = createString();

	int i, del = 1000000, pid = getpid();
	char c;
	for(i = 0; i < 7; i++)
	{
		c = pid / del + '0';
		pid = pid % del;
		del = del / 10;
		putInString(ret, &c);
	}

	return ret;
}
