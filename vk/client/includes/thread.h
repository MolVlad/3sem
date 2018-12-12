#ifndef __THREAD_H__
#define __THREAD_H__

void sigHandler(int nsig);
void threadSidHandler(int nsig);
void *thread();
void createThread();
void deleteThread();

#endif /* __THREAD_H__ */
