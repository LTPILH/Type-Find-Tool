#include "t_foperator.h"
#include "t_trie.h"
#include <unistd.h>
#include <dirent.h>
#include <signal.h>

int main(int argc, char *argv[]) {
	int len = strlen(argv[1]);
	FILE *idxfp = t_fopen(T_FILE_PATH_INDEX, "at+");
	if(t_ftype(argv[1]) == S_IFDIR) {
		DIR *pdir;
		struct dirent *pdirent;
		if((pdir = opendir(argv[1])) == NULL) {
			perror("t_fbuild opendir: ");
			exit(1);
		}
		while((pdirent = readdir(pdir)) != NULL) {
			if(strncmp(pdirent->d_name, ".", 1) == 0)
				continue;
			char *newfile = (char *)malloc(len + strlen(pdirent->d_name) + 5);
			int l_argv1 = strlen(argv[1]);
			if(argv[1][l_argv1-1] == '/') argv[1][--l_argv1] = '\0';
			int l_pdname = strlen(pdirent->d_name);
			int sz = snprintf(newfile, l_argv1 + l_pdname + 5, "%s/%s\n", argv[1], pdirent->d_name);
			fprintf(stderr, "Newfile:%s\n", newfile);
			int ex = t_trie_insert(newfile, sz);
			fprintf(stderr, "Newfile  OUT:%s\n", newfile);
			if(ex == 0) {
				t_flock(idxfp);
				fseek(idxfp, 0L, SEEK_END);
				t_fwrite(newfile, sizeof(char), sz, idxfp);
				t_funlock(idxfp);
			}
		}
	}
	else if(t_ftype(argv[1]) == S_IFREG && len >= 3 && argv[1][len - 2] == '.' && (argv[1][len - 1] == 'c' || argv[1][len - 1] == 'h')) {
		//printf("%s\n", argv[1]);
	}
	else { 
		//printf("%s\n", argv[1]);
	}
	return 0;
}
