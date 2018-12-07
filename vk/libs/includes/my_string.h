#ifndef __MY_STRING_H__
#define __MY_STRING_H__

#define INIT_STRING_SIZE 10
#define TRESHOLD_STRING_SIZE 80
#define AFTER_TRESHOLD_STRING_SIZE 40

typedef char Data;
typedef struct
{
	Data * data;

	int currentSize;
	int maxSize;
} String;

String * createString();
void deleteString(String * string);
int scanStringFromStream(int stream, String * string, int maxNum);
int printStringToStream(int stream, String * string);
void clearString(String * string);
String * copyString(String * original);
int stringCompare(String * first, String * second);

void putInString(String * string, Data * data);

#endif /* __MY_STRING_H__ */
