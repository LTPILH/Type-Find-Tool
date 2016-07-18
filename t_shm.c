#include "t_shm.h"

int excrshm(key_t key) {
	int shmid = -1;
	if((shmid = shmget(key, SEGSIZE, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
		return 0;
	}
	else {
		return -1;
	}
}
int getshm(key_t key) {
	int shmid = -1;
	if((shmid = shmget(key, SEGSIZE, IPC_CREAT | IPC_EXCL | 0666)) == -1) {

		if((shmid = shmget(key, SEGSIZE, 0)) == -1) {
			perror("t_shm shmget: ");
			exit(1);
		}
	}
	return shmid;
}
char *openshm(int shmid) {
	char *segptr = NULL;
	if(*(segptr = shmat(shmid, 0, 0)) == -1) {
		perror("t_shm shmat: ");
		exit(1);
	}
	return segptr;
}

void writeshm(int shmid, int mv, char *text, int len) {
	char *segptr = openshm(shmid);
	strncpy(segptr + mv, text, len);
}
void zeroshm(int shmid, int mv, int len) {
	char *segptr = openshm(shmid);
	bzero(segptr + mv, len);
}
char *readshm(int shmid, int mv, int len) {
	char *segptr = openshm(shmid);
	char *buf = (char *)malloc(len + 1);
	if(buf == NULL) {
		fprintf(stderr, "t_shm readshm: malloc failed\n");
	}
	strncpy(buf, segptr + mv, len);
	buf[len] = '\0';
	return buf;
}
int getsize(int shmid) {
	struct shmid_ds buf;
	if(shmctl(shmid, IPC_STAT, &buf) == -1) {
		perror("t_shm getsize: ");
		exit(1);
	}
	return buf.shm_segsz;
}
void removeshm(int shmid) {
	if(shmctl(shmid, IPC_RMID, 0) == -1) {
		perror("t_shm shmctl: ");
		exit(1);
	}
}
