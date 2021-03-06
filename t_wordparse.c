#include "t_wordparse.h"
#include "t_foperator.h"
#include "t_trie.h"
#include <string.h>


void t_wordparse(char *path, char *segptr) {
	FILE *fp = t_fopen(path, "rt");
	FILE *tp = t_fopen(TYFDS, "at");
	FILE *lp = t_fopen(T_FILELIST, "at");
	char line[LINESIZE], pline[LINESIZE], result[LINESIZE << 1];
	char *ptr;
	char pattern[20];
	int pattlen = 0;

	while(t_freadline(line, LINESIZE, fp) != NULL) {
		if((ptr = t_trim(line)) == NULL) continue;
		int len = strlen(ptr), mv;
		if((mv = t_beginwith(ptr, len, "#include", 8)) >= 0) {
			if(t_get_inc(pline, ptr) == -1) continue;
			if((len = t_gethfile(line, pline)) != -1) {
				int exist = t_trie_insert(segptr, line, len - 1);
				if(exist == 0) {
					t_flock(lp);
					fseek(lp, 0L, SEEK_END);
					t_fwrite(line, 1, len, lp);
					t_funlock(lp);
				}
			}
			continue;
		}
		else if((mv = t_beginwith(ptr, len, "#define", 7)) >= 0) {
			if(t_get_def(pline, ptr + mv + 1) == -1) continue;
			if(t_endwith(ptr, len, "typedef", 7) >= 0) {
				strncpy(pattern, pline + 4, strlen(pline) - 4);
				pattlen = 1;
			}
		}
		else if((mv = t_beginwith(ptr, len, "typedef", 7)) >= 0) {
			if(t_get_tef(pline, ptr + mv + 1) == -1) continue;
		}
		else if((mv = t_beginwith(ptr, len, "struct", 6)) >= 0) {
			if(t_get_stc(pline, ptr + mv + 1) == -1) continue;
		}
		else if(pattlen > 0 && (mv = t_beginwith(ptr, len, pattern, strlen(pattern)) >= 0)) {
			if(t_get_tef(pline, ptr + mv + 1) == -1) continue;
		}
		else if(t_isfunc(pline, ptr, len) == -1) {
			continue;
		}
		int sz = snprintf(result, LINESIZE << 1, "%s\n\t<-- %s\n", pline, path);
		t_flock(tp);
		fseek(tp, 0L, SEEK_END);
		t_fwrite(result, 1, sz, tp);
		t_funlock(tp);
	}
	fclose(fp);
	fclose(tp);
	fclose(lp);
}

int t_gethfile(char *path, char *hname) {
	char cmd[30];
	snprintf(cmd, 30, "locate %s", hname);
	FILE *pp = popen(cmd, "r");
	if(pp == NULL) {
		perror("popen");
		return -1;
	}
	while(t_freadline(path, LINESIZE, pp) != NULL) {
		int plen = strlen(path);
		path[--plen] = '\0';
		if(t_beginwith(path, plen, "/usr/include", 12) == -1) continue;
		if(t_fexist(path) == 0) {
			pclose(pp);
			path[plen++] = '\n';
			path[plen] = '\0';
			return plen;
		}
	}
	pclose(pp);
	//fprintf(stderr, "Can't find head file: %s\n", hname);
	return -1;
}

int t_beginwith(char *s, int slen, char *p, int plen) {
	int i = 0, j = 0;
	while(i < slen) {
		if(s[i] == ' ') {
			i++;
			continue;
		}
		if(s[i] != p[j]) return -1;
		i++;
		j++;
		if(j == plen) return i - 1;
	}
	return -1;
}

int t_endwith(char *s, int slen, char *p, int plen) {
	int i = slen - 1, j = plen - 1;
	while(i >= 0) {
		if(s[i] != p[j]) return -1;
		i--;
		j--;
		if(j == -1) return i + 1;
	}
	return -1;
}

int t_isid(char ch) {
	return (isalpha(ch) || isdigit(ch) || (ch == '_'));
}

char *t_trim(char *src) {
	int len = strlen(src);
	while(len > 0 && (src[len - 1] == ' ' || src[len - 1] == '\t' || src[len - 1] == '\n')) len--;
	src[len] = '\0';
	int i;
	for(i = 0; i < len; i++) {
		if(src[i] != ' ' && src[i] != '\t' && src[i] != '\n')
			return (src + i);
	}
	return NULL;
}

int t_get_inc(char *dst, char *src) {
	int i, sz = 0, slen = strlen(src);
	for(i = 0; i < slen; i++) {
		if(src[i] == '<') break;
	}
	if(i == slen) return -1;
	i++;
	while(i < slen && src[i] != '>') {
		dst[sz++] = src[i++];
	}
	if(i == slen) return -1;
	dst[sz] = '\0';
	return sz;
}

int t_get_def(char *dst, char *src) {
	int i, sz = 0, slen = strlen(src);
	dst[sz++] = '<'; dst[sz++] = 'D'; dst[sz++] = '>'; dst[sz++] = ' ';
	for(i = 0; i < slen; i++) {
		if(t_isid(src[i])) break;
	}
	if(i == slen) return -1;
	while(i < slen && t_isid(src[i])) {
		dst[sz++] = src[i++];
	}
	while(i < slen && !t_isid(src[i])) i++;
	if(i == slen) return -1;
	dst[sz] = '\0';
	return sz;
}

int t_get_tef(char *dst, char *src) {
	int i, sz = 0, slen = strlen(src);
	dst[sz++] = '<'; dst[sz++] = 'T'; dst[sz++] = '>'; dst[sz++] = ' ';
	for(i = 0; i < slen; i++) {
		if(src[i] == ';') break;
	}
	if(i == slen) return -1;
	while(i > 0 && t_isid(src[i - 1])) --i;
	while(src[i] != ';') {
		dst[sz++] = src[i++];
	}
	dst[sz] = '\0';
	return sz;
}

int t_get_stc(char *dst, char *src) {
	int i, sz = 0, slen = strlen(src);
	dst[sz++] = '<'; dst[sz++] = 'S'; dst[sz++] = '>'; dst[sz++] = ' ';
	for(i = 0; i < slen; i++) {
		if(t_isid(src[i]))
			break;
	}
	while(i < slen && t_isid(src[i])) {
		dst[sz++] = src[i++];
	}
	dst[sz] = '\0';
	while(i < slen && (src[i] == ' ' || src[i] == '\t')) i++;
	if(i < slen && src[i] != '{') return -1;
	return sz;
}

int t_isfunc(char *dst, char *src, int len) {
	int i, sz = 0;
	dst[sz++] = '<'; dst[sz++] = 'F'; dst[sz++] = '>'; dst[sz++] = ' ';
	int cnt = 0;
	for(i = 0; i < len; i++) {
		if(src[i] == ' ' || src[i] == '\t' || src[i] == '(') break;
		if(!t_isid(src[i]) && (cnt == 0 || src[i] != '*')) return -1;
		if(t_isid(src[i])) cnt++;
	}
	for( ; i < len; i++) {
		if(src[i] == ' ' || src[i] == '\t') continue;
		break;
	}
	if(i == len) return -1;
	cnt = 0;
	int wds = 0, have = 0;
	while(i < len) {
		if(t_isid(src[i]) || src[i] == '*' || src[i] == ',' || src[i] == '.') {
			i++;
			continue;
		}
		if(src[i] == ' ' || src[i] == '\t') {
			if(i && t_isid(src[i - 1]) && !have) wds++;
			i++;
			continue;
		}
		if(src[i] == '(') {
			cnt++;
			have = 1;
		}
		else if(src[i] == ')') {
			cnt--;
			if(cnt == 0) break;
		}
		else return -1;
		i++;
	}
	if(cnt != 0 || have == 0 || wds > 2) return -1;
	int j;
	for(j = 0; j <= i; j++) dst[sz++] = src[j];
	dst[sz] = '\0';
	i++;
	while(i < len && (src[i] == ' ' || src[i] == '\t')) i++;
	if(i < len && (src[i] != ';' || src[i] != '{')) return -1;
	return sz;
}
