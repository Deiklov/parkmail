///ИЗ2 посвящено приобретению навыков системной разработки на C
///и работе с внешними библиотеками. В качестве результата ИЗ2 ожидается:
///* грамотное разбиение проекта на файлы;
///* наличие двух реализаций – последовательной и параллельной,
///оформленных в виде статической и динамической библиотеки,
///а также тестирующей программы, которая будет сравнивать на разных
///входных данных результаты обеих реализаций между собой;
///* максимальная утилизация ресурсов процессора при параллельной обработке данных путём использования нескольких процессов или потоков;
///* продуманные структуры данных в целях экономии оперативной памяти;
///* реализация алгоритмов, эффективно взаимодействующих с кэш-памятью.
///
///Сравните и выведите в консоль время работы последовательного и параллельного
///с использованием нескольких процессов алгоритмов, каждый из которых выделяет
///в динамической памяти символьный массив размером 100 Мб и выполняет поиск в тексте
///максимальной последовательности цифр.
#include <stdio.h>
#include "stdlib.h"
#include <time.h>
#include "string.h"
#include <stdbool.h>


bool check_digit(char *arr, char *arr_symbol, int index) {
    for (int j = 26; j < strlen(arr_symbol); ++j)
        if (arr[index] == arr_symbol[j])
            return true;
    return false;
}

char *gen_arr(size_t arrsize, const char *arr_symbol) {
    char *arr = (char *) malloc(arrsize * sizeof(char));
    for (int i = 0; i < arrsize - 2; ++i)
        arr[i] = arr_symbol[rand() % 36];
    arr[arrsize - 2] = arr_symbol[rand() % 26];///чтобы при проверке края произошло выделение последовательности
    arr[arrsize - 1] = '\0';
    return arr;
}

double seqcheck(char *name) {
    clock_t start = clock();
    char arr_symbol[] = "qwertyuiopasdfghjklzxcvbnm0123456789";
    const size_t arrsize = 1024 * 1024;
    int l_ind = -1;
    size_t max_len_number = 0, curr_size = 0;
    bool flag = false;///флаг для сбрасывания максимальной последовательности
    FILE* f=fopen(name,"r");
    char* arr = (char *) malloc(arrsize * sizeof(char));
    for (int i = 0; i < arrsize; ++i)
        fscanf(f,"%c",&arr[i]);
    fclose(f);
    // if (!arr) arr=gen_arr(arrsize, arr_symbol);
    for (int i = 0; i < strlen(arr); ++i) {
        if (check_digit(arr, arr_symbol, i)) {
            curr_size++;
            flag = true;
            if (i == strlen(arr) - 1 && max_len_number < curr_size) {
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
    //    printf("%c", buffer[i]);
    }
    free(buffer);
    //puts("");
    clock_t end = clock();
    double time=(double)(end - start) / CLOCKS_PER_SEC;
    return  time;
    printf("All time to work %f seconds \n", (double) (end - start) / CLOCKS_PER_SEC);
    //return buffer;
}