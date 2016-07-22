#include "t_fbuild.h"

void t_fbuild(char *pathname, char *segptr) {
	int len = strlen(pathname);
	FILE *lp = t_fopen(T_FILELIST, "at");

	printf("Reading %s ...\n", pathname);

	if(t_isdir(pathname)) {
		struct dirent *pdirent;
		DIR *pdir = t_dopen(pathname);

		while((pdirent = readdir(pdir)) != NULL) {
			if(strncmp(pdirent->d_name, ".", 1) == 0)
				continue;
			char npath[LINESIZE];
			while(pathname[len - 1] == '/' || pathname[len - 1] == ' ' || pathname[len - 1] == '\n' || pathname[len - 1] == '\t')
				pathname[--len] = '\0';
			int sz = snprintf(npath, LINESIZE, "%s/%s\n", pathname, pdirent->d_name);
			int exist = t_trie_insert(segptr, npath, sz - 1);
			//fprintf(stderr, "Newfile  OUT:%s, ex=%d\n", npath, exist);
			if(exist == 0) {
				t_flock(lp);
				fseek(lp, 0L, SEEK_END);
				t_fwrite(npath, sizeof(char), sz, lp);
				t_funlock(lp);
			}
		}
	}
	else if(t_isreg(pathname) && len >= 3 && pathname[len - 2] == '.' && (pathname[len - 1] == 'c' || pathname[len - 1] == 'h')) {
		//printf("%s\n", pathname);
		t_wordparse(pathname, segptr);
	}
	else { 
		//printf("%s\n", pathname);
	}
	t_fclose(lp);
}
