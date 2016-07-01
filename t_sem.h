#ifndef __T_SEM_H_
#define __T_SEM_H_

#include <sys/sem.h>
#include <sys/types.h>

#define SEM_RESOURCE_MAX 1
#define SEMMSL 2

union semun
{
	int val;				// value for SETVAL
	struct semid_ds *buf;		// buffer for IPC_STAT & IPC_SET
	unsigned short int *array;		// array for GETALL & SETALL
	struct seminfo *__buf;		// buffer for IPC_INFO
};

void opensem(int *sid, key_t key);
void createsem(int *sid, key_t key, int members);
void locksem(int sid, int member);
void unlocksem(int sid, int member);
void removesem(int sid);
unsigned short get_member_count(int sid);
int getval(int sid, int member);

#endif
