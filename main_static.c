#include "single_proc.h"
#include "multi_proc.h"

int main() {
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
    puts("Enter sequences");
    for (size_t i = 0; i < arrsize; ++i)
        scanf("%c", &buffer[i]);
    char *res = seqcheck(buffer, arrsize);
    if (!res) {
        puts("Handle error");
        return 0;
    }
    char *res2 = check_seq_multi(buffer, arrsize);
    printf("Single: %s\n", res);
    printf("Multi: %s\n", res2);
    free(res);
    free(res2);
    free(buffer);
    return 0;
}