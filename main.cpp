// https://github.com/raspberrypi/pico-sdk
// https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf#page=15

#include <cmath>

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

    constexpr int precision = 25;
    int level_max = floor(4095 / precision);
    int denominator = level_max * (level_max + 1);

    while (true) {
        adc_select_input(0);
        uint16_t pot_0 = adc_read();

        adc_select_input(1);
        uint16_t pot_1 = adc_read();

        int coarse = floor(pot_0 / precision);
        int fine = floor(pot_1 / precision);

        int level = coarse * level_max + fine;
        float pct = (float)level / denominator;

        printf(
            "POT_0: %04d, POT_1: %04d, level_0: %04d, level_1: %04d, level: %05d, pct: %f\n",
            pot_0, pot_1,
            coarse, fine, level, pct
        );

        sleep_ms(200);
    }

    return 0;
}
