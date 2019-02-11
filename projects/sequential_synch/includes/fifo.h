#ifndef __FIFO_H__
#define __FIFO_H__

int openOrCreateFIFO(const char * name);
void getID(char * id);
int writeID(int fd, char * id);
int readID(int fd, char * id);

#endif /* __FIFO_H__ */
