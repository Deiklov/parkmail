#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

int main() {
    char *dlpath = "libmulti_dyn_lib.so";
    void *library = dlopen(dlpath, RTLD_LAZY);
    if (!library) {
        printf("dlopen_erorr");
        return 0;
    }
    char *(*check_seq)(char *, size_t);
    char *fname = "check_seq_multi";
    check_seq = dlsym(library, fname);
    if (!check_seq) {
        puts("import_err");
        return 0;
    }
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
    for (size_t i = 0; i < arrsize; ++i)
        scanf("%c", &buffer[i]);
    char *res = check_seq(buffer, arrsize);
    if (!res) {
        puts("Handle error");
        return 0;
    }
    printf("%s\n", res);
    free(buffer);
    dlclose(library);
    return 0;
}