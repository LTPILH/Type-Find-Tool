#include "t_foperator.h"
#include "t_trie.h"
#include <unistd.h>
#include <dirent.h>

void usage();

int main(int argc, char *argv[]) {
	if(argc < 2) usage();
	if(strncmp(argv[1], "-b", 2) == 0) {
		if(argc != 3) usage();
		int len = strlen(argv[2]);
		if(t_ftype(argv[2]) == S_IFDIR) {
			DIR *pdir;
			struct dirent *pdirent;
			if((pdir = opendir(argv[2])) == NULL) {
				perror("t_fbuild opendir: ");
				exit(1);
			}
			while((pdirent = readdir(pdir)) != NULL) {
				if(strncmp(pdirent->d_name, ".", 1) == 0)
					continue;
				char *newfile = (char *)malloc(len + strlen(pdirent->d_name) + 1);
				int sz = snprintf(newfile, sizeof(char), "%s/%s", argv[2], pdirent->d_name);
				newfile[sz] = 0;
				int ex = t_trie_insert(newfile, sz);
				if(ex == 0) {
					int pid;
					if((pid = fork()) < 0) {
						perror("t_fbuild fork: ");
						exit(1);
					}
					else if(pid == 0) {
						if(execlp(argv[0], argv[0], argv[1], newfile, NULL) == -1) {
							perror("t_fbuild execlp: ");
							exit(1);
						}
					}
				}
			}
		}
		else if(t_ftype(argv[2]) == S_IFREG && len >= 3 && argv[2][len - 2] == '.' && (argv[2][len - 1] == 'c' || argv[2][len - 1] == 'h')) {
			printf("%s\n", argv[2]);
		}
	}
	else if(strncmp(argv[1], "-h", 2) == 0) {
		usage();
	}
	else {
		if(argc != 3) usage();

	}
	return 0;
}
void usage() {
	fprintf(stderr, "tman - A type find tool\n");
	fprintf(stderr, "\nUSAGE: tman -b <filepath>\n");
	fprintf(stderr, "       -h\n");
	fprintf(stderr, "       <type> <filepath>\n");
	exit(1);
}
