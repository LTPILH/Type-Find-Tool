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
	char *segptr = -1;
	if((segptr = shmat(shid, 0, 0)) == -1) {
		perror("t_shm shmat: ");
		exit(1);
	}
}

writeshm(int shmid, int mv, char *text, int len) {
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
	strncpy(buf, segptr + mv, len);
	buf[len] = '\0';
	return buf;
}
removeshm(int shmid) {
	if(shmctl(shmid, IPC_RMID, 0) == -1) {
		perror("t_shm shmctl: ");
		exit(1);
	}
}
