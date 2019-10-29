#include <stdbool.h>
#ifndef IZ2_STRUCT_H
#define IZ2_STRUCT_H
typedef struct {
    size_t begin;
    size_t len;
    size_t l_len, r_len, m_len, m_ind;
    char *arr;
    bool ffull;
} segment;
#define ALLERR "Memory allocation error"
#endif //IZ2_STRUCT_H