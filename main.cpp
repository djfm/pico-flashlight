// https://github.com/raspberrypi/pico-sdk
// https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf#page=15

#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

constexpr int POT_0 = 26;
constexpr int POT_1 = 27;

int main() {
    stdio_init_all();
    printf("PICOLIGHT!\n");

    adc_init();

    adc_gpio_init(POT_0);
    adc_gpio_init(POT_1);

    while (true) {
        adc_select_input(0);
        uint16_t pot_0 = adc_read();

        adc_select_input(1);
        uint16_t pot_1 = adc_read();

        printf("POT_0: %04d, POT_1: %04d\n", pot_0, pot_1);

        sleep_ms(200);
    }

    return 0;
}
