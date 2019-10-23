#include "single_thread.h"

double seqcheck(char *name) {
    clock_t start = clock();
    const size_t arrsize = 1024 *1024*100;
    int l_ind = -1;
    size_t max_len_number = 0, curr_size = 0;
    bool flag = false;///флаг для сбрасывания максимальной последовательности
    FILE* f=fopen(name,"r");
    char* arr = (char *) malloc(arrsize * sizeof(char));
    for (int i = 0; i < arrsize; ++i)
        fscanf(f,"%c",&arr[i]);
    fclose(f);
    for (int i = 0; i < arrsize; ++i) {
        if (isdigit(arr[i])) {
            curr_size++;
            flag = true;
            if (i == arrsize - 1 && max_len_number < curr_size) {
                l_ind = i - curr_size+1;
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

    char *buffer = (char *) malloc(sizeof(char) * max_len_number);
    for (int i = 0; i < max_len_number; ++i) {
        buffer[i] = arr[l_ind + i];
       printf("%c", buffer[i]);
    }
    free(buffer);
    free(arr);
    clock_t end = clock();
    double time=(double)(end - start) / CLOCKS_PER_SEC;
    printf("\nTime is %f\n",time);
    return  time;
}