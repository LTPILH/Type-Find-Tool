#include "t_wordparse.h"
#include "t_foperator.h"
#include <string.h>

void t_wordparse(char *path) {
	FILE *fp = t_fopen(path, "rt");
	FILE *tp = t_fopen(TYFDS, "at");
	FILE *lp = t_fopen(T_FILELIST, "at");
	char line[LINESIZE], pline[LINESIZE], result[LINESIZE << 1];

	while(t_freadline(line, LINESIZE, fp) != NULL) {
		int plen = strlen(line);
		int l = 0, r = plen - 1;
		if((l = t_beginwith(line, plen, "#include", 8)) >= 0) {
			while(l < plen && (line[l] != '<' && line[l] != '\"')) l++;
			while(r >= l && (line[r] != '>' && line[r] != '\"')) r--;
			l++; r--;
			if(l > r) continue;
			int pplen = 0;
			while(l <= r) pline[pplen++] = line[l++];
			pline[pplen] = '\0';
			if((plen = t_gethfile(line, pline)) > 0) {
				t_flock(lp);
				fseek(lp, 0L, SEEK_END);
				t_fwrite(line, sizeof(char), plen, lp);
				t_funlock(lp);
			}
		}
		else if((l = t_beginwith(line, plen, "#define", 7)) >= 0) {
			int pplen = t_trim(pline, line + 7, plen - 7);
			int sz = snprintf(result, LINESIZE << 1, "%-50s <--- %s\n", pline, path);
			t_flock(tp);
			fseek(tp, 0L, SEEK_END);
			t_fwrite(result, sizeof(char), sz, tp);
			t_funlock(tp);
		}
		else if((l = t_beginwith(line, plen, "typedef", 7)) >= 0) {
			int pplen = t_trim(pline, line + 7, plen - 7);
			int sz = snprintf(result, LINESIZE << 1, "%-50s <--- %s\n", pline, path);
			t_flock(tp);
			fseek(tp, 0L, SEEK_END);
			t_fwrite(result, sizeof(char), sz, tp);
			t_funlock(tp);
		}
		else if((l = t_beginwith(line, plen, "struct", 6)) >= 0) {
			int pplen = t_trim(pline, line + 6, plen - 6);
			int sz = snprintf(result, LINESIZE << 1, "%-50s <--- %s\n", pline, path);
			t_flock(tp);
			fseek(tp, 0L, SEEK_END);
			t_fwrite(result, sizeof(char), sz, tp);
			t_funlock(tp);
		}
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
		int pathlen = strlen(path);
		if(t_fexist(path) == FILE_NOEXIST) {
			if(pclose(pp) == -1) {
				perror("pclose");
			}
			return pathlen;
		}
	}
	if(pclose(pp) == -1) {
		perror("pclose");
	}
	//fprintf(stderr, "Can't find head file: %s\n", hname);
	return -1;
}

int t_beginwith(char *s, int slen, char *p, int plen) {
	int i, j, pos;
	for(i = 0; i < slen - 1; i++)
		if(s[i] != ' ' && s[i] != '\t')
			break;
	for(j = 0; j < plen - 1; j++)
		if(p[j] != ' ' && p[j] != '\t')
			break;
	if(slen - i < plen - j) return -1;
	pos = i;
	while(j < plen) {
		if(s[i] != p[j]) return -1;
		i++;
		j++;
	}
	return pos;
}

int t_trim(char *dst, char *src, int len) {
	int sz = 0;
	int i, j;
	for(i = 0; i < len && src[i] != ' '; i++);
	for(j = len - 1; j >= i && (src[j] == ' ' || src[j] == '\n'); j--);
	while(i <= j) {
		dst[sz++] = src[i++];
	}
	dst[sz] = '\0';
	return sz;
}
