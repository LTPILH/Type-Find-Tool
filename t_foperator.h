#ifndef __T_FOPERATOR_H_
#define __T_FOPERATOR_H_

#include <sys/stat.h>
#include <sys/file.h>
#include <stdio.h>
#include <dirent.h>

#define TYFDS "./tyfds"
#define T_FILELIST "./t_filelist"
#define FILE_NOEXIST -1
#define LINESIZE 300

int t_fexist(const char *file_path);

FILE *t_fopen(const char *file_path, const char *mode);

DIR *t_dopen(const char *file_path);

void t_fclose(FILE *);

void t_fclear(const char *file_path);

char *t_freadline(char *buf, int sz, FILE *fp);

int t_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *fp);

void t_fremove(const char *file_path);

int t_isdir(char *file_path);

int t_isreg(char *file_path);

ino_t t_getinode(char *file_path);

void t_flock(FILE *fp);

void t_funlock(FILE *fp);

#endif
