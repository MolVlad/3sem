#ifndef __MY_STRING_H__
#define __MY_STRING_H__

#define INIT_STRING_SIZE 10
#define TRESHOLD_STRING_SIZE 80
#define AFTER_TRESHOLD_STRING_SIZE 40
#define OUTPUT_STREAM 1

typedef char Data;
typedef struct
{
	Data * data;

	int currentSize;
	int maxSize;
} String;

String * createString();
void deleteString(String * string);
int scanStringFromStream(int stream, String * string);
void printStringToStream(int stream, String * string);
void clearString(String * string);
String * copyString(String * original);
Flag areStringSame(String * first, String * second);

void copyStringElement(Data * destination, Data * element);
void putInString(String * string, Data * data);

#endif /* __MY_STRING_H__ */
