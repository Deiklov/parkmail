#include "multi_thread.h"
#include "unistd.h"
#include "wait.h"

void init_segment(segment *segm, int _begin, size_t _len, char *arr) {
    segm->begin = _begin;
    segm->len = _len;
    segm->m_ind = 0;
    segm->l_len = 0;
    segm->r_len = 0;
    segm->m_len = 0;
    segm->arr = arr;
}

void thread_routine(char *arr, int deskript, int begin, size_t len) {
    segment *segm = (segment *) malloc(sizeof(segment));
    if (segm == NULL) {
        printf(ALLERR);
        return -1;
    }
    init_segment(segm, begin, len, arr);
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
    write(deskript, segm, sizeof(segment));
    free(segm);
}


work_res *check_seq_multi(char *name, size_t arrsize) {
    clock_t start = clock();
    // const int q_segm = 20, size_segm = 1024 * 1024 * 5;
    // const size_t arrsize = q_segm * size_segm;
    const proc_count = system("cat /proc/cpuinfo|grep processor|wc -l");
    if (proc_count < 1) {
        printf("System error");
        return -1;
    }
    size_t size_segm = arrsize % proc_count == 0 ? arrsize / proc_count : arrsize / proc_count + 1;

    int max_len = 0, max_ind = -1;
    FILE *f = fopen(name, "r");
    char *arr = (char *) malloc(arrsize * sizeof(char));
    if (arr == NULL) return -1;
    for (int i = 0; i < arrsize; ++i)
        fscanf(f, "%c", &arr[i]);
    fclose(f);
    int fd[2], status;
    pipe(fd);
    segment *segm = (segment *) malloc(proc_count * sizeof(segment));
    pid_t *pid = (pid_t *) malloc(proc_count * sizeof(pid_t));
    for (int i = 0; i < proc_count; ++i) {
        //будет столько процессов сколько сегментов
        if (pid[i] = fork() == -1) {
            printf("Fork error");
            return -1;
        }
        if (pid[i] == 0) {
            close(fd[0]);
            thread_routine(arr, fd[1], i * size_segm, size_segm);
            exit(EXIT_SUCCESS);
        } else {
            //close(fd[1]);
            if (i == proc_count - 1) {
                close(fd[1]);
                for (int j = 0; j < proc_count; ++j) {
                    waitpid(pid[j], &status, 0);
                    read(fd[0], &segm[j], sizeof(segment));
                    if (j == 0) {
                        max_len = segm[0].m_len;
                        max_ind = segm[0].m_ind;
                        continue;
                    }
                    if (segm[j - 1].r_len + segm[j].l_len > max_len) {
                        max_ind = segm[j].begin - segm[j - 1].r_len;///суммируем с краев
                        max_len = segm[j - 1].r_len + segm[j].l_len;
                    }
                    if (segm[j].m_len > max_len) {///проверяем центр, может он больше чем макс.
                        max_ind = segm[j].m_ind;
                        max_len = segm[j].m_len;
                    }
                }
                free(pid);
            }
        }
    }
    char *buffer = (char *) calloc(max_len, sizeof(char));
    //memset(buffer, max_len, '\0');
    for (int i = max_ind; i < max_ind + max_len; ++i) {
        buffer[i - max_ind] = arr[i];
        printf("%c", arr[i]);
    }
    free(arr);
    // free(buffer);
    clock_t end = clock();
    double time = (double) (end - start) / CLOCKS_PER_SEC;
    printf("\nTime is %f\n", time);
    work_res *res = (work_res *) malloc(sizeof(work_res));
    res->time = time;
    res->seq = buffer;
    return res;
}