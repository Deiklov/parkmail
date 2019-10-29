#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

int main() {
    char *dlpath = "libdyn_lib.so";
    void *library = dlopen(dlpath, RTLD_LAZY);
    if (!library) {
        printf("dlopen_erorr");
        return 0;
    }
    dlerror();
    char *(*check_seq)(char *, size_t);
    char *fname = "check_seq_multi";
    check_seq = dlsym(library, fname);
//    if (!check_seq) {
//        puts("import_err");
//        return 0;
//    }
    size_t arrsize = 0;
    puts("Enter size of sequence");
    scanf("%ld", &arrsize);
    if (arrsize < 0) {
        puts("Very short sequence");
        return 0;
    }
    char *buffer = (char *) calloc(arrsize, sizeof(char));
    if (!buffer) {
        printf(ALLERR);
        return 0;
    }
    puts("Enter sequence");
    for (size_t i = 0; i < arrsize; ++i)
        scanf("%c", &buffer[i]);
    char *res = check_seq(buffer, arrsize);
    check_seq = dlsym(library, "seqcheck");
    char *res2 = check_seq(buffer, arrsize);
    if (!res) {
        puts("Handle error");
        return 0;
    }
    printf("Multi: %s\n", res);
    printf("Single: %s\n", res2);
    free(buffer);
    free(res);
    free(res2);
    dlclose(library);
    return 0;
}