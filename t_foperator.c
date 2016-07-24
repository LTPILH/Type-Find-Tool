#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "t_foperator.h"

int t_fexist(const char *file_path) {
	return access(file_path, F_OK);
}

FILE *t_fopen(const char *file_path, const char *mode) {
	FILE *fp = fopen(file_path, mode);
	if(fp == NULL) {
		perror("t_fopen");
		exit(1);
	}
	return fp;
}

DIR *t_dopen(const char *file_path) {
	DIR *pdir;
	if((pdir = opendir(file_path)) == NULL) {
		perror("t_dopen");
		exit(1);
	}
	return pdir;
}

void t_fclose(FILE *fp) {
	if(fclose(fp) == EOF) {
		perror("t_flocse");
		exit(1);
	}
}

void t_fclear(const char *file_path) {
	FILE *fp = t_fopen(file_path, "w");
	t_fclose(fp);
}

char *t_freadline(char *buf, int sz, FILE *fp) {
	return fgets(buf, sz, fp);
}

int t_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *fp) {
	int n;
	if((n = fwrite(ptr, size, nmemb, fp) <= 0)) {
		perror("t_fwrite");
		exit(1);
	}
	return n;
}

void t_fremove(const char *file_path) {
	if(t_fexist(file_path) == FILE_NOEXIST)
		return;
	if(remove(file_path) == -1) {
		perror("t_fremove");
		exit(1);
	}
}

int t_isdir(char *file_path) {
	struct stat tmp;

	if(stat(file_path, &tmp) == -1) {
		perror("t_isdir");
		exit(1);
	}
	return (tmp.st_mode & S_IFMT) == S_IFDIR ? 1 : 0;
}

int t_isreg(char *file_path) {
	struct stat tmp;

	if(stat(file_path, &tmp) == -1) {
		perror("t_isreg");
		exit(1);
	}
	return (tmp.st_mode & S_IFMT) == S_IFREG ? 1 : 0;
}

ino_t t_getinode(char *file_path) {
	struct stat tmp;

	if(stat(file_path, &tmp) == -1) {
		perror("t_getinode");
		exit(1);
	}
	return tmp.st_ino;
}

void t_flock(FILE *fp) {
	int fd = fileno(fp);
	if(flock(fd, LOCK_EX) == -1) {
		perror("t_flock");
		exit(1);
	}
}

void t_funlock(FILE *fp) {
	int fd = fileno(fp);
	if(flock(fd, LOCK_UN) == -1) {
		perror("t_funlock");
		exit(1);
	}
}

