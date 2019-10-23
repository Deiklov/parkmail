#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "single_thread.c"
#include "multi_thread.c"

TEST(test_seq, test_name) {
    ASSERT_LT(check_seq_multi("../input_data.txt")->time,seqcheck("../input_data.txt")->time);
    ASSERT_STREQ(check_seq_multi("../input_data.txt")->seq,seqcheck("../input_data.txt")->seq);

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
