#ifndef __SHM_H__
#define __SHM_H__

int createShm(key_t key);
Data * attachMemory(int shmid);
int connectToShm(key_t key);

#endif /* __SHM_H__ */
