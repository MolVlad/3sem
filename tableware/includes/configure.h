#ifndef __CONFIGURE_H__
#define __CONFIGURE_H__

#define TABLEWARE_TIMES "txt/tableware_times.txt"
#define TABLEWARE_LIST "txt/tableware_list.txt"
#define CALLBOARD_FIFO "users"

#define MAX_SIZE_NAME 100
#define SIZE_ID 4
#define PERMISSION 0777
#define SPACE_ON_THE_TABLE 1

#define CHECK(nameFunction, retValue)				\
do {								\
	if(retValue == -1)					\
	{							\
		printf("%s failure\n", nameFunction);		\
		exit(1);					\
	}							\
	else							\
		printf("%s succeeded\n", nameFunction);		\
} while(0)							\

#endif /* __CONFIGURE_H__ */
