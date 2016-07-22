#include "t_trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

char *t_trie_init() {
	key_t key = ftok(".", 's');
	t_createshm(key);
	int shmid = t_openshm(key);
	char *segptr = t_linkshm(shmid);
	int sz = 1;
	t_writeshm(segptr, 0, (char *)&sz, sizeof(sz));
	int semid;
	t_createsem(&semid, key, 1, 1);
	t_clearshm(segptr, sizeof(sz), NODESIZE);
	return segptr;
}

int t_trie_getmv(int id, int num, int type) {
	return sizeof(int) * (1 + id * NODENUM * 2 + num * 2 + type);
}

int t_trie_getval(char *segptr, int mv) {
	int val;
	t_readshm(segptr, mv, (char *)&val, sizeof(int));
	return val;
}

void t_trie_setval(char *segptr, int mv, int val) {
	t_writeshm(segptr, mv, (char *)&val, sizeof(int));
}

int t_trie_idx(char c) {
	return (int)c;
}

int t_trie_insert(char *const segptr, char *name, int len) { // -1 exist
	if(len <= 0) return -1;
	int i, u = 0;
	key_t key = ftok(".", 's');
	int semid;
	t_opensem(&semid, key);
	int sz;
	t_readshm(segptr, 0, (char *)&sz, sizeof(int));
	t_locksem(semid, 0);
	int c = 0, mv;
	for(i = 0; i < len; i++) {
		c = t_trie_idx(name[i]);
		mv = t_trie_getmv(u, c, 0);
		int v = t_trie_getval(segptr, mv);
		if(v != 0) {
			u = v;
			continue;
		}
		t_clearshm(segptr, sizeof(int) * (1 + sz * NODENUM * 2), NODESIZE);
		t_trie_setval(segptr, mv, sz);
		u = sz++;
	}
	t_trie_setval(segptr, 0, sz);
	int cnt = t_trie_getval(segptr, mv + 1);
	t_trie_setval(segptr, mv + 1, cnt + 1);
	t_unlocksem(semid, 0);
	return (cnt == 0 ? 0 : -1);
}

void t_trie_free(char *segptr) {
	key_t key = ftok(".", 's');
	int shmid = t_openshm(key);
	t_blinkshm(segptr);
	t_removeshm(shmid);
	int semid;
	t_opensem(&semid, key);
	t_removesem(semid);
}
