#include "profiling.hpp"
#include <unistd.h>
#include <utils.hpp>

int main (void) {
    {
        PROFILING_SCOPE("Main");
        {
            PROFILING_SCOPE("Test1");
            sleep(1);
        }

        {
            PROFILING_SCOPE("Test2");
            {
                PROFILING_SCOPE("Test3");
                sleep(1);
            }
            sleep(2);
        }
    }
    PROFILING_PRINT();
    return 0;
}
