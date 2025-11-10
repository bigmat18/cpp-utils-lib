#include <utils.hpp>

void gtest(int value) {
    massert(value < 5, "a less then 5");
}

void ftest(int value) {
    gtest(value);
}

int main (void) {
    int a = 5;
    massert(a >= 5, "a greater or equal then 5");
    ftest(a);
    return 0;
}
