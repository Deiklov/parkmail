#include "gtest/gtest.h"

extern "C" {
#include "single_proc.h"
#include "multi_proc.h"
}

TEST(test_seq, single_seq) {
    const size_t arrsize = 50;
    char *buffer = "ioisirowo29g939iifis7374284fi2i2iif829249249294528";
    char *res = seqcheck(buffer, arrsize);
    char *res2 = seqcheck("rr564tyr446646", 14);
    EXPECT_STREQ(res, "829249249294528");
    EXPECT_STREQ(res2, "446646");
    EXPECT_STREQ(check_seq_multi(NULL, 0), "");
    free(res);
    free(res2);

}

TEST(test_seq, multi_seq) {
    const size_t arrsize = 50;
    char *buffer = "ioisirowo29g939iifis7374284fi2i2iif829249249294528";
    char *res = check_seq_multi(buffer, arrsize);
    EXPECT_STREQ(res, "829249249294528");
    char *res2 = check_seq_multi("rr564tyr446646", 14);
    EXPECT_STREQ(res2, "446646");
    free(res);
    free(res2);

}

TEST(test_compare, multi_vs_single) {
    const size_t arrsize = 1024 * 1024 * 100;
    char *arr = (char *) calloc(arrsize, sizeof(char));
    srand(time(NULL));
    char *str = "qwertyuiopasdfghjklzxcvbnm0123456789";
    for (size_t i = 0; i < arrsize; i++)
        arr[i] = str[rand() % 36];
    clock_t start = clock();
    char *res = check_seq_multi(arr, arrsize);
    clock_t end = clock();
    printf("%f\n", (double) (end - start) / CLOCKS_PER_SEC);
    start = clock();
    char *res2 = seqcheck(arr, arrsize);
    end = clock();
    printf("%f\n", (double) (end - start) / CLOCKS_PER_SEC);
    EXPECT_STREQ(res, res2);
}

TEST(test_compare, gen_arr_compare) {
    srand(time(NULL));
    const size_t arrsize = 1024 * 1024 * 10;
    char *arr = (char *) calloc(arrsize, sizeof(char));
    char *str = "qwertyuiopasdfghjklzxcvbnm0123456789";
    for (size_t i = 0; i < arrsize; i++)
        arr[i] = str[rand() % 36];
    char *res_single = seqcheck(arr, arrsize);
    char *res_multi = check_seq_multi(arr, arrsize);
    EXPECT_STREQ(res_multi, res_single);
    free(res_single);
    free(res_multi);
    free(arr);
}

TEST(test_split, split_array) {
    {
        int *arr1 = split_array(4, 50);
        int arr2[] = {12, 12, 12, 14};
        for (size_t i = 0; i < 4; ++i)
            EXPECT_EQ(arr1[i], arr2[i]);

    }
    {
        int *arr1 = split_array(4, 3);
        int arr2[] = {3};
        for (size_t i = 0; i < 1; ++i)
            EXPECT_EQ(arr1[i], arr2[i]);
    }
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}