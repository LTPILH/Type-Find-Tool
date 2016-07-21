#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "t_foperator.h"
#include <ctype.h>
#include <string.h>

int t_fexist(const char *file_path) {
	return access(file_path, F_OK);
}

FILE *t_fopen(const char *file_path, const char *mode) {
	FILE *fp = fopen(file_path, mode);
	if(fp == NULL) {
		perror("t_fopen:");
		exit(1);
	}
	return fp;
}

void t_fclose(FILE *fp) {
	if(fclose(fp) == EOF) {
		perror("t_flocse");
		exit(1);
	}
}
void t_fclear(const char *file_path) {
	FILE *fp = t_fopen(file_path, "w");
	if(fclose(fp) == -1) {
		perror("t_fclear:");
		exit(1);
	}
}
char *t_freadline(char *buf, int sz, FILE *fp) {
	return fgets(buf, sz, fp);
}
int t_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *fp) {
	int n;
	if((n = fwrite(ptr, size, nmemb, fp) <= 0)) {
		perror("t_fwrite:");
		exit(1);
	}
	return n;
}

void t_fremove(const char *file_path) {
	if(t_fexist(file_path) == FILE_NOEXIST)
		return;
	if(remove(file_path) == -1) {
		perror("t_fremove:");
		exit(1);
	}
}
off_t t_fsize(char *file_path) {
	struct stat tmp;
	if(stat(file_path, &tmp) == -1) {
		perror("t_fsize stat: ");
		exit(1);
	}
	return tmp.st_size;
}

int t_ftype(char *file_path) {
	struct stat tmp;
	if(stat(file_path, &tmp) == -1) {
		perror("t_ftyep stat");
		FILE *fp = t_fopen("./log", "at+");
		t_flock(fp);
		fseek(fp, 0L, SEEK_END);
		fprintf(fp, "%s\n", file_path);
		t_funlock(fp);
		exit(1);
	}
	return tmp.st_mode & S_IFMT;
}

time_t t_fmtime(char *file_path) {
	struct stat tmp;
	if(stat(file_path, &tmp) == -1) {
		perror("t_fmtime stat: ");
		exit(1);
	}
	return tmp.st_mtime;
}
void t_flock(FILE *fp) {
	int fd = fileno(fp);
	if(flock(fd, LOCK_EX) == -1) {
		perror("t_flock:");
		exit(1);
	}
}

void t_funlock(FILE *fp) {
	int fd = fileno(fp);
	if(flock(fd, LOCK_UN) == -1) {
		perror("t_funlock:");
		exit(1);
	}
}

