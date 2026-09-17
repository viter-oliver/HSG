#include"non_rolling_log.h"
#include <thread>
#include <chrono>

using namespace std;


/*tests*/

void test1() {
    cc_print("hello cpp");
}
void test2(int a) {
    if (a < 3) {
        cc_print("*DO*NOT*REPLY*");
    }
    else {
        cc_print("**REPLY**");
    }

}
void test3() {
    cc_print("*test check*");
    static int i = 0, j = 0;
    if (i > 10) {
        cc_print("i==%d j==%d", i, j++);
        //i = 0;
    }
    i++;
}
namespace tt4 {

    void test4() {
        cc_print("*goodbye*");
    }
    void test1() {
        cc_print("gray is a dog hahaha");
    }
}



int main(int argc, char* argv[]) {

    test2(0);

    while (1) {

        test1();
        test2(4);
        test3();
        tt4::test4();
        tt4::test1();
         this_thread::sleep_for(500ms);
    }

    return 0;
}