#include "t_trie.h"
#include "t_foperator.h"
#include "t_fbuild.h"
#include <signal.h>

const int NP = 100;

void usage();
int clo_son, all_son;
int proc_lim;

int main(int argc, char *argv[]) {
	if(argc == 2 && strncmp(argv[1], "-h", 2) == 0) usage();
	else if(argc == 3 && strncmp(argv[1], "-b", 2) == 0) {
		void sig_chld(int);
		signal(SIGCHLD, sig_chld);

		t_fclear(T_FILELIST);
		t_fclear(TYFDS);

		int pid;
		all_son = clo_son = 0;
		FILE *lp = t_fopen(T_FILELIST, "rt");
		char *segptr = t_trie_init();
		char pathname[NP][LINESIZE];
		int np = 0;
		strncpy(pathname[np++], argv[2], strlen(argv[2]));
		key_t key = ftok(".", 'm');
		t_createsem(&proc_lim, key, 1, 200);

		while(1) {
			while(np--) {
				t_locksem(proc_lim, 0);
				int pathlen = strlen(pathname[np]);
				while(pathname[np][pathlen - 1] == '\n') pathname[np][--pathlen] = 0;
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
					t_fbuild(pathname[np], segptr);
					exit(0);
				}
			}
			np = 0;
			char *ptr;
			do {
				ptr = t_freadline(pathname[np], LINESIZE, lp);
				if(ptr == NULL) {
					if(np) break;
					if(clo_son < all_son) {
						sleep(300);
					}
					else {
						t_trie_free(segptr);
						t_fclose(lp);
						exit(0);
					}
				}
				else np++;
			}while(np < NP);
		}
	}
	else if(argc == 2) {
		char cmd[50], result[LINESIZE];
		snprintf(cmd, 50, "grep -A1 '<.>[^<]*%s' tyfds", argv[1]);
		FILE *pp;
		if((pp = popen(cmd, "r")) == NULL) {
			perror("popen");
			exit(1);
		}
		while(t_freadline(result, LINESIZE, pp) != NULL)
			printf("%s", result);
	}
	else usage();
}
void sig_chld(int signo) {
	pid_t pid;

	while((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
		++clo_son;
		t_unlocksem(proc_lim, 0);
	}
}
void usage() {
	fprintf(stderr, "tman - A type find tool\n");
	fprintf(stderr, "\nUSAGE: tman -b <filepath>\n");
	fprintf(stderr, "       -h\n");
	fprintf(stderr, "       <type>\n");
	exit(1);
}
