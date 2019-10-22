#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "single_thread.c"
#include "multi_thread.c"
#include "dich.cpp"

TEST(test_seq, test_name) {
    ASSERT_EQ(check(),5);
    ASSERT_LT(check_seq_multi("../input_data.txt"),seqcheck("../input_data.txt"));
//    ASSERT_STREQ(seqcheck(""), "");
//    ASSERT_STREQ(seqcheck("llox"), "");
//    ASSERT_STREQ(seqcheck("1llox"), "1");
//    ASSERT_STREQ(seqcheck("1l2lox34"), "34");
//    ASSERT_STREQ(seqcheck("12lox34"), "12");
//    ASSERT_STREQ(seqcheck("1234"), "1234");
//    ASSERT_STREQ(seqcheck("axaxaxa1xaxa"), "1");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}