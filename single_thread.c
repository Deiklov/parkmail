#include "single_thread.h"

work_res *seqcheck(char *arr, size_t arrsize) {
    clock_t start = clock();
    int l_ind = -1;
    size_t max_len_number = 0, curr_size = 0;
    bool flag = false;///флаг для сбрасывания максимальной последовательности
    for (int i = 0; i < arrsize; ++i) {
        if (isdigit(arr[i])) {
            curr_size++;
            flag = true;
            if (i == arrsize - 1 && max_len_number < curr_size) {
                l_ind = i - curr_size + 1;
                max_len_number = curr_size;
            }
        } else {
            if (!flag) continue;///для оптимизации проверки условий
            flag = false;
            if (max_len_number < curr_size) {
                l_ind = i - curr_size;
                max_len_number = curr_size;
            }
            curr_size = 0;
        }
    }

    char *buffer = (char *) calloc(max_len_number, sizeof(char));
    for (int i = 0; i < max_len_number; ++i) {
        buffer[i] = arr[l_ind + i];
        printf("%c", buffer[i]);
    }
    //free(buffer);
    //free(arr);
    clock_t end = clock();
    double time = (double) (end - start) / CLOCKS_PER_SEC;
    printf("\nSingletime is %f\n", time);
    work_res *res = (work_res *) malloc(sizeof(work_res));
    if (res == NULL) {
        printf(ALLERR);
        exit(0);
    }
    res->time = time;
    res->seq = buffer;
    return res;
}