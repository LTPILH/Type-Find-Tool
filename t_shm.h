#ifndef __T_SHM_H
#define __T_SHM_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <strings.h>

#define SEGSIZE 20000000 

int getshm(key_t key);
char *openshm(int shmid);
void writeshm(int shmid, int mv, char *text, int len);
void zeroshm(int shmid, int mv, int len);
char *readshm(int shmid, int mv, int len);
void removeshm(int shmid);

#endif
