#include "multi_thread.h"

void init_segment(segment *segm, int _begin, size_t _len, char *arr) {
    segm->begin = _begin;
    segm->len = _len;
    segm->m_ind = 0;
    segm->l_len = 0;
    segm->r_len = 0;
    segm->m_len = 0;
    segm->arr = arr;
}

void *thread_routine(void *arg) {
    segment *segm = (segment *) arg;
    size_t curr_size = 0;
    bool flag = false, f_begin = true;
    ///идем до предпоследнего элемента
    for (int i = segm->begin; i < segm->begin + segm->len - 1; ++i) {
        if (isdigit(segm->arr[i])) {
            curr_size++;
            flag = true;
            if (i == segm->len - 2) segm->r_len = curr_size;
        } else {
            if (f_begin) {
                segm->l_len = curr_size;
                f_begin = false;///обработка начала, прервется потом
            } else if (curr_size > segm->m_len) {///обработка мида
                segm->m_ind = i - curr_size;
                segm->m_len = curr_size;
            }
            curr_size = 0;
        }
    }
    ///проверка крайней цифры(буквы)
    if (isdigit(segm->arr[segm->len - 1]) && flag) {
        segm->r_len++;
        curr_size++;
    } else
        segm->r_len = 0;
    if (curr_size > segm->m_len) {///обработка мида
        segm->m_ind = segm->r_len - 1 - curr_size;
        segm->m_len = curr_size;
    }
    pthread_exit(0);

}


double check_seq_multi(char *name) {
    clock_t start = clock();
    const int q_segm = 1024, size_segm = 1024;
    const size_t arrsize = q_segm * size_segm;
    FILE *f = fopen(name, "r");
    char *arr = (char *) malloc(arrsize * sizeof(char));
    for (int i = 0; i < arrsize; ++i)
        fscanf(f, "%c", &arr[i]);
    fclose(f);
    segment segm[q_segm];
    pthread_t threads[q_segm];
    for (int j = 0; j < q_segm; ++j) {
        init_segment(&segm[j], j * size_segm, size_segm, arr);
        pthread_create(&threads[j], NULL, thread_routine, &segm[j]);
    }
    for (int i = 0; i < q_segm; ++i) {
        pthread_join(threads[i], NULL);
    }
    int max_len = segm[0].m_len, max_ind = segm[0].m_ind;
    for (int i = 1; i < q_segm; ++i) {
        if (segm[i - 1].r_len + segm[i].l_len > max_len) {
            max_ind = segm[i].begin - segm[i - 1].r_len;///суммируем с краев
            max_len = segm[i - 1].r_len + segm[i].l_len;
        }
        if (segm[i].m_len > max_len) {///проверяем центр, может он больше чем макс.
            max_ind = segm[i].m_ind;
            max_len = segm[i].m_len;
        }
    }
    char *buffer = (char *) malloc(max_len * sizeof(char));
    for (int i = max_ind; i < max_ind + max_len; ++i) {
        buffer[i - max_ind] = arr[i];
        // printf("%c", arr[i]);
    }
    free(arr);
    free(buffer);
    clock_t end = clock();
    double time = (double) (end - start) / CLOCKS_PER_SEC;
    printf("%f\n",time);
    return time;
}