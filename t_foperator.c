#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "t_foperator.h"

int t_fexist(char *file_path) {
	return access(file_path, F_OK);
}

char *t_fread(char *file_path) {
	off_t size = t_fsize(file_path);
	char *buff = (char *)malloc((size + 1) * sizeof(char));
	if(buff == NULL) {
		perror("t_fread malloc: ");
		exit(1);
	}
	FILE *file;
	if((file = fopen(file_path, "r")) == NULL) {
		perror("t_fread fopen: ");
		exit(1);
	}
	size = fread(buff, 1, size, file);
	buff[size] = 0;
	return buff; // don't forget free buff
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
		perror("t_ftype stat: ");
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
