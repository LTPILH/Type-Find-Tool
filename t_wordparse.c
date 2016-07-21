#include "t_woordparse.h"

void t_wordparse(char *path) {
	FILE *fp = t_fopen(pathname, "r");
	char line[LINESIZE], word[LINESIZE >> 1];
	int def = 0, tef = 0, stc = 0, inc = 0;
	while(fgets(line, LINESIZE, fp) != NULL) {
		int ptr = 0, plen = strlen(line);
		while((ptr = getword(word, &wlen, line + ptr, plen)) < 0) {
			if(wlen == 0) continue;
			if(strncmp(word, "typedef", 7) == 0) {
				tef = 1;
			}
			else if(strncmp(word, "#define", 7) == 0) {
				def = 1;
			}
			else if(strncmp(word, "struct", 6) == 0) {
				stc = 1;
			}
			else if(strncmp(word, "#include", 8) == 0) {
				inc = 1;
			}
			else if(tef > 0) {

			}
			else if(def > 0) {
			}
			else if(stc > 0) {
			}
			else if(inc > 0) {
			}
		}
	}
}

char *t_fparse_h(char *hname) {
	char path[LINESIZE];
	char cmd[30];
	snprintf(cmd, 30, "locate %s", hname);
	FILE *pp = popen(cmd, "r");
	if(pp == NULL) {
		perror("popen");
		return;
	}
	while(fgets(path, LINESIZE, pp) != NULL) {
		int pathlen = strlen(path);
		path[--pathlen] = '\0';
		if(t_fexist(path) == 0) {
			if(pclose(pp) == -1) {
				perror("pclose");
			}
			return path;
		}
	}
	if(pclose(pp) == -1) {
		perror("pclose");
	}
	fprintf(stderr, "Can't find head file: %s\n", hname);
	return NULL;
}

int getword(char *word, int *len, char *sentence, int n) {
	int sz = 0, i;
	for(i = 0; i < n; i++) {
		if(sentence[i] == ' ' || sentence[i] == '\n' || sentence[i] == '\t') {
			if(sz == 0) continue;
			word[sz] = '\0';
			(*len) = sz;
			return i;
		}
		word[sz++] = sentence[i];
		if(!isdigit(sentence[i]) && !isalpha(sentence[i]) && sentence[i] != '_') {
			break;
		}
	}
	word[sz] = '\0';
	(*len) = sz;
	return sz == 0 ? -1 : n;
}
