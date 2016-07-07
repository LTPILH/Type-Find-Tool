#include "t_trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

void t_trie_init() {
	key_t key = ftok(".", 's');
	int shmid = getshm(key);
	int sz = 1;
	writeshm(shmid, 0, (char *)&sz, sizeof(sz));
	int rdcnt = 0;
	writeshm(shmid, sizeof(sz), (char *)&rdcnt, sizeof(rdcnt));
	int semid;
	createsem(&semid, key, 3); // 0 - read, 1 - write, 2 - add cnt
	writeshm(shmid, sizeof(sz) + sizeof(rdcnt), (char *)&semid, sizeof(semid));
	zeroshm(shmid, sizeof(sz) + sizeof(rdcnt) + sizeof(semid), sigma_size * 2);
}

int t_trie_getval(int shmid, int i, int j, int k) {
	char *buf = readshm(shmid, sizeof(int)*(3+i*sigma_size*2+j*2+k), sizeof(int));
	return *(int *)buf;
}

void t_trie_setval(int shmid, int i, int j, int k, int v) {
	writeshm(shmid, sizeof(int)*(3+i*sigma_size*2+j*2+k), (char *)&v, sizeof(int));
}

int t_trie_idx(char c) {
	return (int)c;
}

int t_trie_insert(char *name, int len) { // -1 exist
	if(len <= 0) return -1;
	int i, u = 0, preu;
	key_t key = ftok(".", 's');
	int shmid = getshm(key);
	int exist = -1;
	int sz = *(int *)readsem(shmid, 0, sizeof(int));
	int *rdcnt = (int *)readsem(shmid, sizeof(int), sizeof(int));
	int semid = *(int *)readsem(shmid, sizeof(int)*2, sizeof(int));
	locksem(semid, 0);
	if(*rdcnt == 0) locksem(semid, 1);
	*rdcnt++;
	unlocksem(semid, 0);
	int c = 0;
	for(i = 0; i < len; i++) {
		c = t_trie_idx(name[i]);
		int v = t_trie_getval(shmid, u, c, 0);
		if(v != 0) {
			preu = u;
			u = v;
			continue;
		}
		locksem(semid, 0);
		*rdcnt--;
		if(*rdcnt == 0) unlocksem(semid, 1);
		unlocksem(semid, 0);
		locksem(semid, 1);
		v = t_trie_getval(shmid, u, c, 0);
		if(v != 0) {
			unlocksem(semid, 1);
			locksem(semid, 0);
			if(*rdcnt == 0) locksem(semid, 1);
			*rdcnt++;
			unlocksem(semid, 0);
			continue;
		}
		while(i < len) {
			c = t_trie_idx(name[i]);
			zeroshm(shmid, sizeof(int)*3+sz*sigma_size*2, sigma_size * 2);
			t_trie_setval(shmid, u, c, 0, sz);
			preu = u;
			u = sz++;
			i++;
		}
		int cnt = t_trie_getval(shmid, preu, c, 1);
		t_trie_setval(shmid, preu, c, 1, cnt + 1);
		writesem(shmid, 0, (char *)&sz, sizeof(int));
		unlocksem(semid, 1);
		return 0;
	}
	locksem(semid, 0);
	*rdcnt--;
	if(*rdcnt == 0) unlocksem(semid, 1);
	unlocksem(semid, 0);
	locksem(semid, 2);
	int cnt = t_trie_getval(shmid, preu, c, 1);
	t_trie_setval(shmid, preu, c, 1, cnt + 1);
	int ret = (cnt == 0 ? 0 : -1);
	unlocksem(semid, 2);
	return ret;
}
