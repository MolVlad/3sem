#ifndef __PRECOMP_H__
#define __PRECOMP_H__

/* Includes ----------------------------------------------------------------------*/

// Вы идейно потались сделать нечто вроде precompiled header: некий заголовочный файл, где собраны часто используемые header'ы
// Пока это учебное упражнение - все отлично, в если у вас будет большой проект, который компилируется больше часа, то 
// такой header содержащий кучу других - плохая идея, т.к. значительно увеличит время сборки. Для расширения кругозора почитайте про
// precompiled headers.

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#define MAX_TIME 5

#endif /* __PRECOMP_H__ */
