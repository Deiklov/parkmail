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
    segm->ffull = false;
}

void thread_routine(char *arr, int deskript, int begin, size_t len) {
    segment *segm = (segment *) malloc(sizeof(segment));
    if (segm == NULL) {
        printf(ALLERR);
        return;
    }
    init_segment(segm, begin, len, arr);
    size_t curr_size = 0;
    bool flag = false, f_begin = true, ffull = false;
    ///идем до предпоследнего элемента
    for (int i = segm->begin; i < segm->begin + segm->len-1 ; ++i) {
        char symbol=segm->arr[i];
        if (isdigit(segm->arr[i])) {
            curr_size++;///тек длина
            flag = true;///флаг что в числовой последовталеьности
            if (i == segm->begin+segm->len - 2) segm->r_len = curr_size;///кидаем в право текущее кол-во
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
    if (isdigit(segm->arr[segm->begin+ segm->len - 1]) && flag) {
        segm->r_len++;
        curr_size++;///если ласт цифра не сбивает последовтаельность
        if (curr_size == len) {
            segm->ffull = true;
            segm->r_len = curr_size;
            segm->l_len = curr_size;
        }///если весь сегмент цифры
    } else {///если предпослдений буква
        if (isdigit(segm->arr[segm->begin+segm->len - 1])) segm->r_len = 1;
        else segm->r_len = 0;
        if (curr_size > segm->m_len) {///обработка мида
            segm->m_ind = segm->r_len - 1 - curr_size;
            segm->m_len = curr_size;
        }
    }
    if(write(deskript, segm, sizeof(segment))==-1){
            printf("Pipe write error");
            exit(0);
        };
    free(segm);
}


work_res *check_seq_multi(char *arr, size_t arrsize) {
    clock_t start = clock();
    const proc_count = 4;
    //const proc_count = system("cat /proc/cpuinfo|grep processor|wc -l");
    if (proc_count < 1) {
        printf("System error");
        return -1;
    }
    size_t size_segm = arrsize % proc_count == 0 ? arrsize / proc_count : arrsize / proc_count + 1;
    int max_len = 0, max_ind = -1;
    if (arr == NULL) return -1;
    int fd[2], status;
    pipe(fd);
    //на 1 сегмент 1 процесс
   segment *segm = (segment *) malloc(proc_count * sizeof(segment));
    //segment segm[4];
    pid_t *pid = (pid_t *) malloc(proc_count * sizeof(pid_t));
    for (int i = 0; i < proc_count; ++i) {
       pid[i] = fork();
        if ( pid[i]== -1) {
            printf("Fork error");
            return -1;
        }
//        if (i == proc_count - 1)
//               thread_routine(arr, fd[1], i * size_segm, arrsize - size_segm * i);
//            else thread_routine(arr, fd[1], i * size_segm, size_segm);
        if (pid[i] == 0) {
            close(fd[0]);
            if (i == proc_count - 1)
                thread_routine(arr, fd[1], i * size_segm, arrsize - size_segm * i);
            else thread_routine(arr, fd[1], i * size_segm, size_segm);
            exit(EXIT_SUCCESS);
        } else {
            if (i == proc_count - 1) {
                int templen;
                close(fd[1]);
                waitpid(pid[0], &status, 0);
                read(fd[0], &segm[0], sizeof(segment));
                max_len = segm[0].m_len > segm[0].l_len ? segm[0].m_len : segm[0].l_len;///выбор между левым или центром
                max_ind = max_len == segm[0].m_len ? segm[0].m_ind : 0;///либо 0 либо центр
                for (int j = 1; j < proc_count; ++j) {
                    waitpid(pid[j], &status, 0);
                    read(fd[0], &segm[j], sizeof(segment));
                    templen = segm[j].l_len;///длина на стыке левого и правого концов
                    for (int k = j; k > 0; k--) {///проверка если до этого идет несколько сегментов с цифрами
                        templen += segm[k - 1].r_len;
                        if (!segm[k - 1].ffull) break;
                    }
                    if (templen > max_len) {
                        max_ind = segm[j].begin + segm[j].l_len - templen;///суммируем с краев
                        max_len = templen;
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
   // free(arr);
    // free(buffer);
    clock_t end = clock();
    double time = (double) (end - start) / CLOCKS_PER_SEC;
    printf("\nTime is %f\n", time);
    work_res *res = (work_res *) malloc(sizeof(work_res));
    res->time = time;
    res->seq = buffer;
    return res;
}