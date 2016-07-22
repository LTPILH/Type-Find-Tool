#include "t_shm.h"

void t_createshm(key_t key) {
	int shmid = -1;
	if((shmid = shmget(key, SEGSIZE, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
		shmid = t_openshm(key);
		t_removeshm(shmid);
		if((shmid = shmget(key, SEGSIZE, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
			perror("t_createshm");
			exit(1);
		}
	}
}

int t_openshm(key_t key) {
	int shmid;
	if((shmid = shmget(key, SEGSIZE, 0)) == -1) {
		perror("t_openeshm");
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
	strncpy(text, segptr + mv, len);
	text[len] = '\0';
}

void t_writeshm(char *segptr, int mv, char *text, int len) {
	strncpy(segptr + mv, text, len);
}

void t_clearshm(char *segptr, int mv, int len) {
	bzero(segptr + mv, len);
	printf("t_clearshm end\n");
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
