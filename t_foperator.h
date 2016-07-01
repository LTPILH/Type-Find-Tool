#ifndef __T_FOPERATOR_H_
#define __T_FOPERATOR_H_
#include <sys/stat.h>

int t_fexist(char *file_path);

char *t_fread(char *file_path);

off_t t_fsize(char *file_path);

int t_ftype(char *file_path);

time_t t_fmtime(char *file_path);

#endif
