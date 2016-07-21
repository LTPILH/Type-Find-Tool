#ifndef __T_FOPERATOR_H_
#define __T_FOPERATOR_H_

#include <sys/stat.h>
#include <sys/file.h>
#include <stdio.h>

#define T_FILE_PATH_INDEX "./t_filelist"
#define FILE_NOEXIST -1
#define LINESIZE 200

int t_fexist(const char *file_path);

FILE *t_fopen(const char *file_path, const char *mode);

void t_fclose(FILE*);

void t_fclear(const char *file_path);

char *t_freadline(char *buf, int sz, FILE *fp);

int t_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *fp);

void t_fremove(const char *file_path);

off_t t_fsize(char *file_path);

int t_ftype(char *file_path);

time_t t_fmtime(char *file_path);

void t_flock(FILE *fp);

void t_funlock(FILE *fp);

#endif
