#include "debug.hpp"
#include <map>
#include <print>
#include <utils.hpp>
#include <vector>

int main (void) {
    int a = 5;
    

    if (a == 4) {
        BP_STOP();
    } else {
        BP_STOP();
    }

    int b = 5;
    BP_COND(b == 5, _(a), _(b));

    std::vector<int> v = {a, b};
    BP_STOP(_(v));

    std::map<int, int> m= {{a, b}};
    BP_STOP(_(m));

    std::tuple<int, float, const int> t = {3, 4.9, 2};
    BP_STOP(_(t));

    return 0;
}
