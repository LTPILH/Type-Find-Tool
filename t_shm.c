#include "t_shm.h"

int t_createshm() {
	int shmid = -1;
	if((shmid = shmget(IPC_PRIVATE, SEGSIZE, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
		perror("t_createshm");
		exit(1);
	}
	return shmid;
}

char *t_linkshm(int shmid) {
	char *segptr = NULL;
	if((segptr = shmat(shmid, NULL, 0)) == (void *)-1) {
		perror("t_linkshm");
		exit(1);
	}
	return segptr;
}

void t_blinkshm(const char *segptr) {
	if(shmdt(segptr) == -1) {
		perror("t_blinkshm");
	}
}

void t_readshm(char *segptr, int mv, char *text, int len) {
	memcpy(text, segptr + mv, len);
}

void t_writeshm(char *segptr, int mv, char *text, int len) {
	memcpy(segptr + mv, text, len);
}

void t_clearshm(char *const segptr, int mv, int len) {
	bzero(segptr + mv, len);
}

int t_getsize(int shmid) {
	struct shmid_ds buf;

	if(shmctl(shmid, IPC_STAT, &buf) == -1) {
		perror("t_getsize");
		exit(1);
	}
	return buf.shm_segsz;
}

void t_removeshm(int shmid) {
	if(shmctl(shmid, IPC_RMID, 0) == -1) {
		perror("t_removeshm");
		exit(1);
	}
}
