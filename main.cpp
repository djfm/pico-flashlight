// https://github.com/raspberrypi/pico-sdk
// https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf#page=15

#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    stdio_init_all();
    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
    return 0;
}