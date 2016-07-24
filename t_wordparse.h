#ifndef __T_WORDPARSE_H_
#define __T_WORDPARSE_H_

void t_wordparse(char *pathname, char *segptr);

int t_gethfile(char *path, char *hname);

int t_beginwith(char *s, int slen, char *p, int plen);

int t_endwith(char *s, int slen, char *p, int plen);

char *t_trim(char *src);

int t_isid(char ch);

int t_get_inc(char *dst, char *src);

int t_get_def(char *dst, char *src);

int t_get_tef(char *dst, char *src);

int t_get_stc(char *dst, char *src);

int t_isfunc(char *dst, char *src, int len);

#endif
