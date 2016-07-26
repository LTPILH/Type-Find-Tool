#include "t_trie.h"
#include "t_foperator.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int t_trie_init() {
	key_t key = ftok(".", 's');
	t_createshm(key);
	int shmid = t_createshm(key);
	char *segptr = t_linkshm(shmid);
	int sz = 1;
	t_writeshm(segptr, 0, (char *)&sz, sizeof(sz));
	int semid;
	t_createsem(&semid, key, 1, 1);
	t_clearshm(segptr, sizeof(sz), NODESIZE);
	t_blinkshm(segptr);
	return shmid;
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
	return c - '0';
}

int t_trie_insert(char *const segptr, const char *name, int len) { // -1 exist
	if(len <= 0) return -1;
	char path[LINESIZE];
	strncpy(path, name, len);
	path[len] = '\0';
	long long ino = t_getinode(path);
	int plen = snprintf(path, LINESIZE, "%lld", ino);
	int i, u = 0;
	key_t key = ftok(".", 's');
	int sz;
	int semid;
	t_opensem(&semid, key);
	t_locksem(semid, 0);
	t_readshm(segptr, 0, (char *)&sz, sizeof(int));
	int c = 0, mv;
	for(i = 0; i < plen; i++) {
		c = t_trie_idx(path[i]);
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
	mv = t_trie_getmv(u, c, 1);
	int cnt = t_trie_getval(segptr, mv);
	int exist = (cnt == 0 ? 0 : -1);
	if(cnt == 0) t_trie_setval(segptr, mv, cnt + 1);
	t_unlocksem(semid, 0);
	return exist;
}

void t_trie_free(int shmid) {
	t_removeshm(shmid);
	key_t key = ftok(".", 's');
	int semid;
	t_opensem(&semid, key);
	t_removesem(semid);
}
