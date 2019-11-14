#ifndef IZ2_MULTI_PROC_H
#define IZ2_MULTI_PROC_H

#include "stdlib.h"
#include <time.h>
#include "string.h"
#include "pthread.h"
#include <stdbool.h>
#include "stdio.h"
#include <ctype.h>
#include<math.h>
#include "struct.h"

char *check_seq_multi(char *, size_t);

void child_proc(const char *, int, int, size_t);

void init_segment(segment *, int, size_t);

#endif //IZ2_MULTI_PROC_H