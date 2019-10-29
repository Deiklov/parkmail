#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "single_thread.c"
#include "multi_thread.c"

TEST(test_seq, test_name) {
    const size_t arrsize=1024*1024*100;
        char* arr=(char*)calloc(arrsize, sizeof(char));
        memset(arr,'\0',arrsize);
        FILE *f=fopen("../input_data.txt","r");
        for(int i=0;i<arrsize;i++)
            fscanf(f,"%c",&arr[i]);
        fclose(f);
   // EXPECT_LT(check_seq_multi(arr,arrsize)->time,seqcheck(arr,arrsize)->time);
    EXPECT_STREQ(check_seq_multi(arr,arrsize)->seq,seqcheck(arr,arrsize)->seq);

}
TEST(test_seq, test_name2) {
    //EXPECT_LT(check_seq_multi(arr,arrsize)->time,seqcheck(arr,arrsize)->time);
    EXPECT_STREQ(check_seq_multi("",0)->seq,seqcheck("",0)->seq);

}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
