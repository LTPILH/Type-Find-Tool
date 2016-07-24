#ifndef __T_SEM_H_
#define __T_SEM_H_

#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define SEM_RESOURCE_MAX 2000
#define SEMMSL 1

union semun
{
	int val;				// value for SETVAL
	struct semid_ds *buf;		// buffer for IPC_STAT & IPC_SET
	unsigned short int *array;		// array for GETALL & SETALL
	struct seminfo *__buf;		// buffer for IPC_INFO
};

void t_createsem(int *sid, key_t key, int members, int semval);

void t_opensem(int *sid, key_t key);

void t_locksem(int sid, int member);

void t_unlocksem(int sid, int member);

void t_removesem(int sid);

unsigned short t_get_member_count(int sid);

#endif
