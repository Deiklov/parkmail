#include "single_thread.h"
#include "multi_thread.h"
#include "stdio.h"
int main(){
    seqcheck("../input_data.txt");
    check_seq_multi("../input_data.txt");
    return 0;
}