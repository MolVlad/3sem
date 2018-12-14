#ifndef __FIFO_CONFIG_H__
#define __FIFO_CONFIG_H__

#define FILE_FOR_KEY "txt/key.txt"
#define GENERAL_FIFO "txt/general_fifo"
#define PRIVATE_FIFO_NAME_SIZE 6

key_t getTheKey(const char * keyFileName, int id);
int createFIFO(const char * name);
int openFIFO(const char * name);

#endif /* __FIFO_CONFIG_H__ */
