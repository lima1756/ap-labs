#include "logger.h"

int main() {
    infof("This is info: %d\n", 5);
    warnf("This is warning: %c\n", 'c');
    errorf("This is error: %d\n", 3.14);
    panicf("This is panic: %s\n", "AAAAAAAA");
    return 0;
}
