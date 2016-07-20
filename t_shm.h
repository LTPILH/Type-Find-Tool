#ifndef __T_SHM_H
#define __T_SHM_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define SEGSIZE 20000000 

int getshm(key_t key);
char *openshm(int shmid);
void writeshm(char *segptr, int mv, char *text, int len);
void zeroshm(char *segptr, int mv, int len);
char *readshm(char *segptr, int mv, int len);
int getsize(int shmid);
void removeshm(int shmid);

#endif
