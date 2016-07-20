#ifndef __T_TRIE_H_
#define __T_TRIE_H_

#include "t_sem.h"
#include "t_shm.h"

#define maxnode 20000
#define sigma_size 128

void t_trie_init();
int t_trie_getval(char *segptr, int i, int j, int k);
void t_trie_setval(char *segptr, int i, int j, int k, int v);
int t_trie_idx(char c);
int t_trie_insert(char *segptr, char *name, int len);
void t_trie_free();

#endif
