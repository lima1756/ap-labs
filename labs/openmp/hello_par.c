#include <stdio.h>
#include <omp.h>
#include "logger.h"

int main() {
    #pragma omp parallel
    {
        infof("Hello from process: %d\n", omp_get_thread_num());
    }
    return 0;
}
