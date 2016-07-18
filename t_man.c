#include "t_trie.h"
#include "t_foperator.h"
#include <signal.h>

void usage();
int son_pro;

int main(int argc, char *argv[]) {
	if(argc == 1) usage();
	if(strncmp(argv[1], "-h", 2) == 0) usage();
	if(strncmp(argv[1], "-b", 2) == 0) {
		if(argc != 3) usage();
		int pid;
		son_pro = 0;
		void sig_chld(int);
		signal(SIGCHLD, sig_chld);
		void sig_int(int);
		signal(SIGINT, sig_int);
		t_fclear(T_FILE_PATH_INDEX);
		FILE *idxfp = t_fopen(T_FILE_PATH_INDEX, "rt+");
		t_trie_init();
		char pathname[LINESIZE];
		strncpy(pathname, argv[2], sizeof(pathname));
		while(1) {
			int pathlen = strlen(pathname);
			while(pathname[pathlen - 1] == '\n') pathname[--pathlen] = 0;
			pid = fork();
			son_pro++;
			if(pid < 0) {
				perror("tman fork: ");
				t_trie_free();
				exit(1);
			}
			else if(pid == 0) {
				if(execlp("./t_fbuild", "./t_fbuild", pathname, NULL) == -1) {
					perror("t_man execlp: ");
					exit(1);
				}
			}
			else {
				//fprintf(stderr, "Create : %d, path=%s\n", pid, pathname);
				while(t_freadline(pathname, LINESIZE, idxfp) == NULL) {
					if(son_pro > 0) continue;
					t_trie_free();
					exit(0);
				}
				//fprintf(stderr, "Readline : %s\n", pathname);
			}
		}
	}
}
void sig_chld(int signo) {
	pid_t pid;

	while((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
		son_pro--;
		//fprintf(stderr, "Delete : %d\n", pid);
	}
}
void sig_int(int signo) {
	t_trie_free();
	exit(1);
}
void usage() {
	fprintf(stderr, "tman - A type find tool\n");
	fprintf(stderr, "\nUSAGE: tman -b <filepath>\n");
	fprintf(stderr, "       -h\n");
	fprintf(stderr, "       <type> <filepath>\n");
	exit(1);
}
