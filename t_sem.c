#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "t_sem.h"

void opensem(int *sid, key_t key) {
	if((*sid = semget(key, 0, 0660)) == -1) {
		fprintf(stderr, "Semaphore set does not exist!\n");
		exit(1);
	}
}
void createsem(int *sid, key_t key, int members) {
	int cntr;
	union semun semopts;

	if(members > SEMMSL) {
		fprintf(stderr, "Sorry, max number of semaphore in a set is %d\n", SEMMSL);
		exit(1);
	}
	if((*sid = semget(key, members, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
		fprintf(stderr, "Semaphore set already exists!\n");
		exit(1);
	}
	semopts.val = SEM_RESOURCE_MAX;
	for(cntr = 0; cntr < members; cntr++)
		semctl(*sid, cntr, SETVAL, semopts);
}
void locksem(int sid, int member) {
	struct sembuf sem_lock = {0, -1, 0};
	if(member < 0 || member > get_member_count(sid) - 1) {
		fprintf(stderr, "semaphore member %d out of range\n", member);
		return;
	}
	sem_lock.sem_num = member;
	if((semop(sid, &sem_lock, 1)) == -1) {
		fprintf(stderr, "Lock failed\n");
		exit(1);
	}
}
void unlocksem(int sid, int member) {
	struct sembuf sem_unlock = {member, 1, 0};
	int semval;

	if(member < 0 || member > get_member_count(sid) - 1) {
		fprintf(stderr, "semaphore member %d out of range\n", member);
		return;
	}

	semval = getval(sid, member);
	if(semval == SEM_RESOURCE_MAX) {
		fprintf(stderr, "Semaphore not locked!\n");
		exit(1);
	}
	sem_unlock.sem_num = member;
	if((semop(sid, &sem_unlock, 1)) == -1) {
		fprintf(stderr, "Unlock failed!\n");
		exit(1);
	}
}
void removesem(int sid) {
	if(semctl(sid, 0, IPC_RMID, 0) == -1) {
		perror("t_sem removesem: ");
		exit(1);
	}
}
unsigned short get_member_count(int sid) {
	union semun semopts;
	struct semid_ds mysemds;

	semopts.buf = &mysemds;

	if(semctl(sid, 0, IPC_STAT, semopts) == -1) {
		perror("t_sem get_member_count: ");
		exit(1);
	}
	return semopts.buf->sem_nsems;
}
int getval(int sid, int member) {
	int semval;

	semval = semctl(sid, member, GETVAL, 0);
	return semval;
}
