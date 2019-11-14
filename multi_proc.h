#ifndef IZ2_MULTI_PROC_H
#define IZ2_MULTI_PROC_H

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include<math.h>
#include <unistd.h>
#include <wait.h>
#include <sys/sysinfo.h>
#include <stddef.h>
#include "struct.h"


char *check_seq_multi(char *, size_t);

int *split_array(size_t, size_t);

void child_proc(const char *, int, int, size_t);

segment *read_from_pipe(const int *, segment *, const int);

void init_segment(segment *, size_t , size_t);

char *merge_segment(const char *, const segment *, const int);

#endif //IZ2_MULTI_PROC_H