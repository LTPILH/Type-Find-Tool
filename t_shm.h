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

void t_createshm(key_t key);

int t_openshm(key_t key);

char *t_linkshm(int shmid);

void t_blinkshm(const char *segptr);

void t_readshm(char *segptr, int mv, char *text, int len);

void t_writeshm(char *segptr, int mv, char *text, int len);

void t_clearshm(char *segptr, int mv, int len);

int t_getsize(int shmid);

void t_removeshm(int shmid);

#endif
