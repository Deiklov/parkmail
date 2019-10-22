#ifndef IZ2_MULTI_THREAD_H
#define IZ2_MULTI_THREAD_H
#include "stdlib.h"
#include <time.h>
#include "string.h"
#include "pthread.h"
#include <stdbool.h>
#include "stdio.h"
#include <ctype.h>
typedef struct {
    int begin;
    int len;
    int l_len, r_len, m_len, m_ind;
    char *arr;
} segment;
double check_seq_multi(char*);
void *thread_routine(void*);
void init_segment(segment *, int , size_t , char *);
#endif //IZ2_MULTI_THREAD_H
