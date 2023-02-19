// https://github.com/raspberrypi/pico-sdk
// https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf#page=15

#include <cmath>

#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

constexpr int POT_0 = 26;
constexpr int POT_1 = 27;
constexpr int BATTERY_PIN = 28;
constexpr float BATTERY_V_DIVIDER = 25.4375;

uint32_t time_ms() {
    return to_ms_since_boot(get_absolute_time());
}

int main() {
    stdio_init_all();
    printf("PICOLIGHT!\n");

    adc_init();

    adc_gpio_init(POT_0);
    adc_gpio_init(POT_1);
    adc_gpio_init(BATTERY_PIN);

    constexpr float level_max = 4095 * 4096 + 4095;

    auto start = time_ms();
    auto last = start;
    uint64_t loops = 0;

    while (true) {
        loops++;

        adc_select_input(0);
        uint16_t coarse = adc_read();

        adc_select_input(1);
        uint16_t fine = adc_read();

        int level = coarse * 4096 + fine;
        float pct = level / level_max;



        adc_select_input(2);
        uint16_t battery = adc_read();
        float battery_v_divided = battery * 3.3 / 4095;
        float battery_v = battery_v_divided * BATTERY_V_DIVIDER;



        auto now = time_ms();
        if (now > last + 500) {
            int loops_per_second = loops * 1000 / (now - start);
            printf("dt: %d, loops / s: %d\n", now - last, loops_per_second);

            printf(
                "coarse: %04d, fine: %04d, level: %05d, pct: %f\n",
                coarse, fine, level, pct
            );

            printf(
                "battery: %04d, battery_v_divided: %1.5f, battery_v: %2.5f\n\n",
                battery, battery_v_divided, battery_v
            );

            last = now;
        }
    }

    return 0;
}
