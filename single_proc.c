#include "single_proc.h"

char *seqcheck(const char *arr, size_t arrsize) {
    size_t l_ind = -1;
    size_t max_len_number = 0, curr_size = 0;
    for (size_t i = 0; i < arrsize; ++i)
        if (isdigit(arr[i])) {
            curr_size++;
            if (max_len_number < curr_size) {
                l_ind = i - curr_size + 1;
                max_len_number = curr_size;
            }
        } else
            curr_size = 0;
    char *buffer = (char *) calloc(max_len_number, sizeof(char));
    if (buffer == NULL) {
        printf(ALLERR);
        return NULL;
    }
    for (size_t i = 0; i < max_len_number; ++i)
        buffer[i] = arr[l_ind + i];
    return buffer;
}