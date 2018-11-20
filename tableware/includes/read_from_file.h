#ifndef __READ_FROM_FILE__
#define __READ_FROM_FILE__

#include"list.h"
#include"libs.h"

void readTimesList(List * listTimes);
void readTablewareList(List * listTableware, List * listTimes);
FILE * fileOpenAndCheck(const char * fileName);

#endif /* __READ_FROM_FILE__ */
