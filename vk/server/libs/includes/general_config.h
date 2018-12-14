#ifndef __GENERAL_CONFIG_H__
#define __GENERAL_CONFIG_H__

#define FILE_LIST "txt/list.txt"

#define PERMISSION 0666

typedef enum Flag
{
	FALSE = 0,
	TRUE,
} Flag;

#define STDIN 1
#define STDOUT 0

#define CHECK(nameFunction, retValue)				\
do								\
{								\
	if(retValue < 0)					\
	{							\
		perror(nameFunction);				\
		kill(getpid(), SIGINT);				\
	}							\
}while(0)							\

#endif /* __GENERAL_CONFIG_H__ */
