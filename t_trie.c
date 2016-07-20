#include "t_trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

void t_trie_init() {
	key_t key = ftok(".", 's');
	int shmid = getshm(key);
	char *segptr = openshm(shmid);
	int sz = 1;
	writeshm(segptr, 0, (char *)&sz, sizeof(sz));
	int semid;
	createsem(&semid, key, 1); // 0 - read, 1 - write, 2 - add cnt
	zeroshm(segptr, sizeof(sz), sizeof(int) * sigma_size * 2);
}

int t_trie_getval(char *segptr, int i, int j, int k) {
	char *buf = readshm(segptr, sizeof(int)*(1+i*sigma_size*2+j*2+k), sizeof(int));
	return *(int *)buf;
}

void t_trie_setval(char *segptr, int i, int j, int k, int v) {
	writeshm(segptr, sizeof(int)*(1+i*sigma_size*2+j*2+k), (char *)&v, sizeof(int));
}

int t_trie_idx(char c) {
	return (int)c;
}

int t_trie_insert(char *segptr, char *name, int len) { // -1 exist
	if(len <= 0) return -1;
	int i, u = 0, preu;
	key_t key = ftok(".", 's');
	int semid;
	opensem(&semid, key);
	int sz = *(int *)readshm(segptr, 0, sizeof(int));
	locksem(semid, 0);
	int c = 0;
	for(i = 0; i < len; i++) {
		c = t_trie_idx(name[i]);
		int v = t_trie_getval(segptr, u, c, 0);
		if(v != 0) {
			preu = u;
			u = v;
			continue;
		}
		c = t_trie_idx(name[i]);
		zeroshm(segptr, sizeof(int) * (1 + sz * sigma_size * 2), sizeof(int) * sigma_size * 2);
		t_trie_setval(segptr, u, c, 0, sz);
		preu = u;
		u = sz++;
	}
	writeshm(segptr, 0, (char *)&sz, sizeof(int));
	int cnt = t_trie_getval(segptr, preu, c, 1);
	t_trie_setval(segptr, preu, c, 1, cnt + 1);
	int exist = (cnt == 0 ? 0 : -1);
	unlocksem(semid, 0);
	return exist;
}
void t_trie_free() {
	key_t key = ftok(".", 's');
	int shmid = getshm(key);
	removeshm(shmid);
	int semid;
	opensem(&semid, key);
	removesem(semid);
}
