#ifndef __CONFIG_H__
#define __CONFIG_H__

#define STDIN 1
#define STDOUT 0

#define NUM_SPACE_TO_BREAK 2

#define PORT 51002

#define HTABLE_SIZE 1
#define HTABLE_STORAGE "txt/htable_storage"

#define BACKLOG 1000

enum MessageType
{
	LOGIN = 0,
	REG,
	MSG,
};

typedef struct
{
	enum MessageType type;

	int loginSize;
	int passwordSize;
	int dataSize;
} HeaderMessageStruct;

#define CHECK(nameFunction, retValue)				\
do								\
{								\
	if(retValue < 0)					\
	{							\
		perror(nameFunction);				\
		exit(1);					\
	}							\
}while(0)							\

typedef enum Flag
{
	FALSE = 0,
	TRUE,
} Flag;

#endif /* __CONFIG_H__ */
