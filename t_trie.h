#ifndef __T_TRIE_H_
#define __T_TRIE_H_

#include "t_sem.h"

struct T_NODE {
	struct T_NODE *childnode[128];
	int cnt;
};

struct T_trie {
	struct T_NODE *t_root;
	key_t key;
	int sem_id;
	int tot;
};

void T_NODE_init(struct T_NODE *node);
void T_trie_init(struct T_trie *trie, char _key);
int t_idx(char c);
int t_insert(struct T_trie *trie, char *name, int len);

#endif
