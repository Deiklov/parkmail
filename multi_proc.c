#include "multi_proc.h"
#include "unistd.h"
#include "wait.h"
#include <sys/sysinfo.h>

void init_segment(segment *segm, int _begin, size_t _len) {
    segm->begin = _begin;
    segm->len = _len;
    segm->m_ind = 0;
    segm->l_len = 0;
    segm->r_len = 0;
    segm->m_len = 0;
    segm->ffull = false;
}

segment *read_from_pipe(const int *fd, segment *segm, const int segm_count) {
    for (int i = 0; i < segm_count; ++i)
        read(fd[2 * i], &segm[i], sizeof(segment));
    return segm;
}

char *merge_segment(const char *arr, const segment *segm, const int segm_count) {
    int templen;
    int max_len = segm[0].m_len > segm[0].l_len ? segm[0].m_len : segm[0].l_len;///выбор между левым или центром
    int max_ind = max_len == segm[0].m_len ? segm[0].m_ind : 0;///либо 0 либо центр
    for (int i = 1; i < segm_count; ++i) {
        templen = segm[i].l_len;///длина на стыке левого и правого концов
        for (int k = i; k > 0; k--) {///проверка если до этого идет несколько сегментов с цифрами
            templen += segm[k - 1].r_len;
            if (!segm[k - 1].ffull) break;
        }
        if (templen > max_len) {
            max_ind = segm[i].begin + segm[i].l_len - templen;///суммируем с краев
            max_len = templen;
        }
        if (segm[i].m_len > max_len) {///проверяем центр, может он больше чем макс.
            max_ind = segm[i].m_ind;
            max_len = segm[i].m_len;
        }
    }
    char *buffer = (char *) calloc(max_len, sizeof(char));
    for (int i = max_ind; i < max_ind + max_len; ++i)
        buffer[i - max_ind] = arr[i];
    return buffer;
}

void child_proc(const char *arr, int deskript, int begin, size_t len) {
    segment segm;
    init_segment(&segm, begin, len);
    size_t curr_size = 0;
    bool f_begin = true;
    for (size_t i = begin; i < begin + len; ++i) {
        if (isdigit(arr[i])) {
            curr_size++;///тек длина
            if (segm.m_len < curr_size) {
                segm.m_ind = i - curr_size + 1;
                segm.m_len = curr_size;
            }
        } else {
            if (f_begin) {
                segm.l_len = curr_size;
                f_begin = false;///обработка начала, прервется потом
            }
            curr_size = 0;
        }
    }
    segm.r_len = curr_size;///обработка правой границы
    if (curr_size == len) {
        segm.ffull = true;
        segm.l_len = len;///правая автоматом пройдет
    }
    if (write(deskript, &segm, sizeof(segment)) == -1) {
        printf("Pipe write error");
        exit(EXIT_FAILURE);
    };
}

int *split_array(int cores, size_t arrsize) {
    if (arrsize < cores) {
        int *len_arr = (int *) calloc(1, sizeof(int));
        len_arr[0] = arrsize;///массив содержащий длины кусков для обработки
        return len_arr;
    } else {
        int len_segm = arrsize / cores;
        int *len_arr = (int *) calloc(cores, sizeof(int));
        for (int i = 0; i < cores - 1; ++i)
            len_arr[i] = len_segm;
        len_arr[cores - 1] = arrsize - (cores - 1) * len_segm;
        return len_arr;
    }
}

char *check_seq_multi(char *arr, size_t arrsize) {
    const size_t proc_count = get_nprocs_conf();
    if (proc_count < 1) {
        printf("System error");
        exit(EXIT_FAILURE);
    }
    int *segm_len = split_array(proc_count, arrsize);
    int segm_count = arrsize < proc_count ? 1 : proc_count;
    int fd[2 * segm_count];
    for (int i = 0; i < segm_count; ++i)
        pipe(&fd[i * 2]);
    pid_t pid[segm_count];
    for (int i = 0; i < segm_count; ++i) {
        pid[i] = fork();
        if (pid[i] == 0) {
            child_proc(arr, fd[2 * i + 1], segm_len[0] * i, segm_len[i]);
            for (int j = 0; j < segm_count; ++j) {
                close(fd[2 * j]);
                close(fd[2 * j + 1]);
            }
            exit(EXIT_SUCCESS);
        } else {
            if (pid[i] < 0) {
                puts("fork error");
                exit(EXIT_FAILURE);
            } else
                close(fd[2 * i + 1]);///закрыли дескрпитор на запись
        }
    }
    int status;
    for (int i = 0; i < segm_count; ++i)
        waitpid(pid[i], &status, 0);
    segment segm[segm_count];
    read_from_pipe(fd, segm, segm_count);
    char *res = merge_segment(arr, segm, segm_count);
    for (int i = 0; i < segm_count; i++)
        close(fd[2 * i]);
    return res;
}