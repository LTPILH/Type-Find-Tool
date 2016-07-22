#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "t_sem.h"

void t_createsem(int *sid, key_t key, int members, int semval) {
	int cntr;
	union semun semopts;

	if(members > SEMMSL) {
		fprintf(stderr, "t_createsem:Max number of semaphore in a set is %d\n", SEMMSL);
		exit(1);
	}
	if((*sid = semget(key, members, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
		t_opensem(sid, key);
		t_removesem(*sid);
		if((*sid = semget(key, members, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
			fprintf(stderr, "t_createsem:Semaphore set already exists!\n");
			exit(1);
		}
	}
	semopts.val = semval;
	for(cntr = 0; cntr < members; cntr++)
		if(semctl(*sid, cntr, SETVAL, semopts) == -1) {
			perror("t_createsem - semctl");
			exit(1);
		}
}

void t_opensem(int *sid, key_t key) {
	if((*sid = semget(key, 0, 0660)) == -1) {
		fprintf(stderr, "t_opensem:Semaphore set does not exist!\n");
		exit(1);
	}
}

void t_locksem(int sid, int member) {
	struct sembuf sem_lock = {0, -1, 0};

	if(member < 0 || member >= t_get_member_count(sid)) {
		fprintf(stderr, "t_locksem:Semaphore member %d out of range\n", member);
		return;
	}
	sem_lock.sem_num = member;
	if((semop(sid, &sem_lock, 1)) == -1) {
		perror("t_locksem - semop");
		//fprintf(stderr, "t_locksem:Lock failed\n");
		exit(1);
	}
}
void t_unlocksem(int sid, int member) {
	struct sembuf sem_unlock = {member, 1, 0};
	int semval;

	if(member < 0 || member >= t_get_member_count(sid)) {
		fprintf(stderr, "t_unlocksem:Semaphore member %d out of range\n", member);
		return;
	}
	sem_unlock.sem_num = member;
	if((semop(sid, &sem_unlock, 1)) == -1) {
		perror("t_unlocksem - semop");
		exit(1);
	}
}
void t_removesem(int sid) {
	if(semctl(sid, 0, IPC_RMID, 0) == -1) {
		perror("t_removesem: ");
		exit(1);
	}
}
unsigned short t_get_member_count(int sid) {
	union semun semopts;
	struct semid_ds mysemds;

	semopts.buf = &mysemds;

	if(semctl(sid, 0, IPC_STAT, semopts) == -1) {
		perror("t_get_member_count");
		exit(1);
	}
	return semopts.buf->sem_nsems;
}
