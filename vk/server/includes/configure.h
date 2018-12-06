#ifndef __CONFIGURE_H__
#define __CONFIGURE_H__

//#define DEBUG_STRING 1
//#define DEBUG_HTABLE 1
//#define DEBUG_BTREE 1

#define HTABLE_SIZE 1
#define HTABLE_STORAGE "txt/htable_storage"

typedef enum Flag
{
	FALSE = 0,
	TRUE,
} Flag;

#endif /* __CONFIGURE_H__ */
