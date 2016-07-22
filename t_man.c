#include "t_trie.h"
#include "t_foperator.h"
#include "t_fbuild.h"
#include <signal.h>

void usage();
int clo_son, all_son;

int main(int argc, char *argv[]) {
	if(argc == 1) usage();
	if(strncmp(argv[1], "-h", 2) == 0) usage();
	if(strncmp(argv[1], "-b", 2) == 0) {
		if(argc != 3) usage();

		void sig_chld(int);
		signal(SIGCHLD, sig_chld);

		t_fclear(T_FILELIST);
		t_fclear(TYFDS);

		int pid;
		all_son = clo_son = 0;
		FILE *lp = t_fopen(T_FILELIST, "rt");
		char *segptr = t_trie_init();
		char pathname[LINESIZE];
		strncpy(pathname, argv[2], sizeof(pathname));

		while(1) {
			int pathlen = strlen(pathname);
			while(pathname[pathlen - 1] == '\n') pathname[--pathlen] = 0;
			++all_son;
			pid = fork();
			if(pid < 0) {
				perror("tman - fork");
				t_trie_free(segptr);
				t_fclose(lp);
				exit(1);
			}
			else if(pid == 0) {
				t_fclose(lp);
				t_fbuild(pathname, segptr);
				exit(0);
			}
			else {
				//fprintf(stderr, "Create : %d, path=%s, all_son=%d, ftell=%d\n", pid, pathname, all_son, ftell(lp));
				char *ptr;
				do {
					//t_flock(idxfp);
					ptr = t_freadline(pathname, LINESIZE, lp);
					//printf("Before clo_son=%d, all_son=%d, ftell=%d\n", clo_son, all_son, ftell(idxfp));
					//ptr = fgets(pathname, LINESIZE, idxfp);
					//t_funlock(idxfp);
					if(ptr == NULL) {
						//printf("clo_son=%d, all_son=%d, ftell=%d\n", clo_son, all_son, ftell(idxfp));
						if(clo_son < all_son) {
							sleep(500);
						}
						else {
							t_trie_free(segptr);
							t_fclose(lp);
							exit(0);
						}
					}
					//printf("After clo_son=%d, all_son=%d, ftell=%d\n", clo_son, all_son, ftell(idxfp));
				}while(ptr == NULL);
				//fprintf(stderr, "Readline : %s\n", pathname);
			}
		}
	}
}
void sig_chld(int signo) {
	pid_t pid;

	while((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
		++clo_son;
	}
}
void usage() {
	fprintf(stderr, "tman - A type find tool\n");
	fprintf(stderr, "\nUSAGE: tman -b <filepath>\n");
	fprintf(stderr, "       -h\n");
	fprintf(stderr, "       <type> <filepath>\n");
	exit(1);
}
