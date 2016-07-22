#ifndef __T_WORDPARSE_H_
#define __T_WORDPARSE_H_

void t_wordparse(char *pathname, char *segptr);

int t_gethfile(char *path, char *hname);

int t_beginwith(char *s, int slen, char *p, int plen);

int t_trim(char *dst, char *src, int len);

#endif
