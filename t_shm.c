#include "t_shm.h"

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
		perror("t_shm shmat");
		exit(1);
	}
	return segptr;
}
void writeshm(char *segptr, int mv, char *text, int len) {
	strncpy(segptr + mv, text, len);
}
void zeroshm(char *segptr, int mv, int len) {
	bzero(segptr + mv, len);
}
char *readshm(char *segptr, int mv, int len) {
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
