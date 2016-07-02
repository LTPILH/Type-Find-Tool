#include "t_trie.h"
#include <stdio.h>
#include <stdlib.h>

void T_NODE_init(struct T_NODE *node) {
	node->cnt = 0;
	int i;
	for(i = 0; i < 128; i++) node->childnode[i] = NULL;
}

void T_trie_init(struct T_trie *trie, char _key) {
	int i;
	trie->tot = 0;
	trie->t_root = (struct T_NODE *)malloc(sizeof(struct T_NODE));
	T_NODE_init(trie->t_root);
	trie->key = ftok(".", _key);
	createsem(&trie->sem_id, trie->key, 3); // 0 - read, 1 - write, 2 - add cnt
}

int t_idx(char c) {
	return (int)c;
}

int t_insert(struct T_trie *trie, char *name, int len) { // -1 exist
	int i;
	struct T_NODE *ptr = trie->t_root;
	int exist = -1;
	locksem(trie->sem_id, 0);
	if(trie->tot == 0) locksem(trie->sem_id, 1);
	trie->tot++;
	unlocksem(trie->sem_id, 0);
	for(i = 0; i < len; i++) {
		int k = t_idx(name[i]);
		if(ptr->childnode[k] != NULL) {
			ptr = ptr->childnode[k];
			continue;
		}
		locksem(trie->sem_id, 0);
		trie->tot--;
		if(trie->tot == 0) unlocksem(trie->sem_id, 1);
		unlocksem(trie->sem_id, 0);
		locksem(trie->sem_id, 1);
		if(ptr->childnode[k] == NULL) {
			unlocksem(trie->sem_id, 1);
			locksem(trie->sem_id, 0);
			if(trie->tot == 0) locksem(trie->sem_id, 1);
			trie->tot++;
			unlocksem(trie->sem_id, 0);
			continue;
		}
		while(i < len) {
			k = t_idx(name[i]);
			ptr->childnode[k] = (struct T_NODE *)malloc(sizeof(struct T_NODE));
			T_NODE_init(ptr->childnode[k]);
			ptr = ptr->childnode[k];
			i++;
		}
		ptr->cnt++;
		unlocksem(trie->sem_id, 1);
		return 0;
	}
	locksem(trie->sem_id, 0);
	trie->tot--;
	if(trie->tot == 0) unlocksem(trie->sem_id, 1);
	unlocksem(trie->sem_id, 0);
	locksem(trie->sem_id, 2);
	int ret = (++ptr->cnt == 1 ? 0 : -1);
	unlocksem(trie->sem_id, 2);
	return ret;
}
