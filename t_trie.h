#ifndef __T_TRIE_H_
#define __T_TRIE_H_

#include "t_sem.h"
#include "t_shm.h"

#define MAXNODE 20000
#define NODENUM 10
#define NODESIZE (NODENUM * 2 * 4)

char *t_trie_init();

int t_trie_getmv(int id, int num, int type);

int t_trie_getval(char *segptr, int mv);

void t_trie_setval(char *segptr, int mv, int val);

int t_trie_idx(char c);

int t_trie_insert(char *const segptr, const char *name, int len);

void t_trie_free();

#endif
