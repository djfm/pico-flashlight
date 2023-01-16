#include <stdio.h>
#include "pico/stdlib.h"

// https://github.com/raspberrypi/pico-sdk

int main() {
    setup_default_uart();
    printf("Hello, world!\n");
    return 0;
}