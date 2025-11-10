#include <utils.hpp>

int main (void) {
    int a = 4;

    if (a == 4) {
        BP_STOP();
    } else {
        BP_STOP();
    }

    int b = 5;
    BP_COND(b == 5, _(a), _(b));
    return 0;
}
