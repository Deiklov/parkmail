//
// Created by andrey on 24.10.2019.
//

#ifndef IZ2_STRUCT_H
#define IZ2_STRUCT_H
typedef struct {
    int begin;
    int len;
    int l_len, r_len, m_len, m_ind;
    char *arr;
} segment;
typedef struct {
    double time;
    char* seq;
} work_res;
#endif //IZ2_STRUCT_H
