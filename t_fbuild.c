#include "t_fbuild.h"

void t_fbuild(char *pathname) {
	int len = strlen(pathname);
	FILE *idxfp = t_fopen(T_FILE_PATH_INDEX, "at");
	if(t_ftype(pathname) == S_IFDIR) {
		DIR *pdir;
		struct dirent *pdirent;
		if((pdir = opendir(pathname)) == NULL) {
			perror("t_fbuild opendir: ");
			exit(1);
		}
		key_t key = ftok(".", 's');
		char *segptr = openshm(getshm(key));
		while((pdirent = readdir(pdir)) != NULL) {
			if(strncmp(pdirent->d_name, ".", 1) == 0)
				continue;
			char *newfile = (char *)malloc(len + strlen(pdirent->d_name) + 5);
			if(pathname[len-1] == '/') pathname[--len] = '\0';
			int l_pdname = strlen(pdirent->d_name);
			int sz = snprintf(newfile, len + l_pdname + 5, "%s/%s\n", pathname, pdirent->d_name);
			if(newfile[0] != '/') fprintf(stderr, "Newfile:%s\n", newfile);
			int ex = t_trie_insert(segptr, newfile, sz - 1);
			//fprintf(stderr, "Newfile  OUT:%s, ex=%d\n", newfile, ex);
			if(ex == 0) {
				t_flock(idxfp);
				fseek(idxfp, 0L, SEEK_END);
				fprintf(idxfp, "%s", newfile);
				//t_fwrite(newfile, sizeof(char), sz, idxfp);
				t_funlock(idxfp);
			}
		}
		if(shmdt(segptr) == -1) {
			perror("shmdt");
			exit(1);
		}
	}
	else if(t_ftype(pathname) == S_IFREG && len >= 3 && pathname[len - 2] == '.' && (pathname[len - 1] == 'c' || pathname[len - 1] == 'h')) {
		//printf("%s\n", pathname);
	}
	else { 
		//printf("%s\n", pathname);
	}
	t_fclose(idxfp);
}
