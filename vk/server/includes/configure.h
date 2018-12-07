#ifndef __CONFIGURE_H__
#define __CONFIGURE_H__

//#define DEBUG_STRING 1
//#define DEBUG_HTABLE 1
//#define DEBUG_BTREE 1

#define HTABLE_SIZE 1
#define HTABLE_STORAGE "txt/htable_storage"

#define STDIN 1
#define STDOUT 0

#define CHECK(nameFunction, retValue)				\
do {								\
	if(retValue < 0)					\
	{							\
		perror(nameFunction);		\
		exit(1);					\
	}							\
	else							\
		printf("%s succeed\n", nameFunction);		\
} while(0)							\

typedef enum Flag
{
	FALSE = 0,
	TRUE,
} Flag;

#endif /* __CONFIGURE_H__ */
