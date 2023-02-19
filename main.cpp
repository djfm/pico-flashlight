// https://github.com/raspberrypi/pico-sdk
// https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf#page=15

#include <cmath>
#include <deque>
#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

constexpr uint8_t POT_0 = 26;
constexpr uint8_t POT_1 = 27;
constexpr uint8_t BATTERY_PIN = 28;
constexpr uint8_t PWM_PIN = 0;
constexpr uint16_t PWM_WRAP = 2048;
constexpr float BATTERY_V_DIVIDER = 25.4375;

static uint32_t clock_speed;
static float pwm_divider;
static float pwm_frequency;

uint32_t time_ms() {
    return to_ms_since_boot(get_absolute_time());
}

void set_duty_cycle(uint pwm_pin, float duty_cycle) {
    auto slice_num = pwm_gpio_to_slice_num(pwm_pin);
    auto channel = pwm_gpio_to_channel(pwm_pin);
    pwm_set_chan_level(slice_num, channel, duty_cycle * PWM_WRAP);
}

void init_pwm(uint pwm_pin, int frequency = 50000) {
    clock_speed = clock_get_hz(clk_sys);
    pwm_divider = (float)clock_speed / (PWM_WRAP * frequency);

    if (pwm_divider < 1) {
        pwm_divider = 1;
    } else if (pwm_divider > 256) {
        pwm_divider = 256;
    }

    pwm_frequency = (float)clock_speed / (pwm_divider * PWM_WRAP);

    gpio_set_function(pwm_pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pwm_pin);
    pwm_set_clkdiv(slice_num, pwm_divider);
    pwm_set_wrap(slice_num, PWM_WRAP);
    set_duty_cycle(pwm_pin, 0);
    pwm_set_enabled(slice_num, true);
}

class MovingAverage {
    float_t sum = 0;
    float_t average = 0;
    uint32_t samples_wanted = 0;
    std::deque<float_t> samples;

    public:
        MovingAverage(int samples_wanted) : samples_wanted(samples_wanted) {

        }

        void add(float_t value) {
            samples.push_back(value);
            sum += value;

            if (samples.size() > samples_wanted) {
                sum -= samples.front();
                samples.pop_front();
            }

            average = sum / samples.size();
        }

        bool ready() {
            return samples.size() >= samples_wanted;
        }

        float_t get() {
            return average;
        }
};

int main() {
    stdio_init_all();
    printf("PICOLIGHT!\n");

    adc_init();

    adc_gpio_init(POT_0);
    adc_gpio_init(POT_1);
    adc_gpio_init(BATTERY_PIN);

    init_pwm(PWM_PIN);

    constexpr float level_max = 4095 * 4096 + 4095;

    auto start = time_ms();
    auto last = start;
    uint64_t loops = 0;

    MovingAverage avg_battery_v(1000);

    while (true) {
        loops++;

        adc_select_input(0);
        uint16_t coarse = adc_read();

        adc_select_input(1);
        uint16_t fine = adc_read();

        int level = coarse * 4096 + fine;
        float pct = level / level_max;

        set_duty_cycle(PWM_PIN, pct);

        adc_select_input(2);
        uint16_t battery = adc_read();
        float_t battery_v_divided = battery * 3.3 / 4095;
        float_t battery_v = battery_v_divided * BATTERY_V_DIVIDER;
        avg_battery_v.add(battery_v);

        auto now = time_ms();
        if (now > last + 5000) {
            int loops_per_second = loops * 1000 / (now - start);
            printf("dt: %d, loops / s: %d\n", now - last, loops_per_second);

            printf(
                "coarse: %04d, fine: %04d, level: %05d, pct: %1.5f\n",
                coarse, fine, level, pct
            );

            printf(
                "pwm_divider: %f, clock_speed: %d, pwm_frequency: %f\n",
                pwm_divider, clock_speed, pwm_frequency
            );

            printf(
                "battery: %04d, battery_v_divided: %1.5f, avg_battery_v: %2.5f\n\n",
                battery, battery_v_divided, avg_battery_v.get()
            );

            last = now;
        }
    }

    return 0;
}
