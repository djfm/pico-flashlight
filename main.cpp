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

    constexpr float level_max = 4095 * 4096 + 4095;

    while (true) {
        adc_select_input(0);
        uint16_t coarse = adc_read();

        adc_select_input(1);
        uint16_t fine = adc_read();

        int level = coarse * 4096 + fine;
        float pct = level / level_max;

        printf(
            "coarse: %04d, fine: %04d, level: %05d, pct: %f\n",
            coarse, fine, level, pct
        );

        sleep_ms(200);
    }

    return 0;
}
