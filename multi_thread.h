#ifndef IZ2_MULTI_THREAD_H
#define IZ2_MULTI_THREAD_H
#include "stdlib.h"
#include <time.h>
#include "string.h"
#include "pthread.h"
#include <stdbool.h>
#include "stdio.h"
#include <ctype.h>
#include "struct.h"
work_res* check_seq_multi(char*);
void thread_routine(char*,int,int,size_t);
void init_segment(segment *, int , size_t , char *);
#endif //IZ2_MULTI_THREAD_H
