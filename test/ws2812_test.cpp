
#include <cstdio>
//
#include "pico/stdio.h"
#include "pico/stdio_usb.h"
#include "pico/stdlib.h"
#include "pico/time.h" // sleep_ms
//
#include "ws2812.h"

static constexpr int ws2812_gpio = 12;

static constexpr int str_len = 4;

static Ws2812 ws2812(ws2812_gpio, str_len);

// each call to set() should be very quick as long as
// number of leds <= fifo depth (8 for 2040 and 2350)
static int32_t set_min_us = INT32_MAX;
static int32_t set_max_us = INT32_MAX;
static int32_t set_sum_us = 0; // for avg
static int32_t set_cnt = 0;    // for avg

static void set(int r, int g, int b)
{
    int32_t t0_us = int32_t(time_us_32());
    ws2812.set(r, g, b);
    int32_t t1_us = int32_t(time_us_32());
    const int32_t set_us = t1_us - t0_us;
    if (set_min_us == INT32_MAX || set_min_us > set_us)
        set_min_us = set_us;
    if (set_max_us == INT32_MAX || set_max_us < set_us)
        set_max_us = set_us;
    set_sum_us += set_us;
    set_cnt++;
}


int main()
{
    stdio_init_all();

#if 1
    while (!stdio_usb_connected())
        tight_loop_contents();
#endif

    sleep_ms(10);

    printf("\n");
    printf("ws2812_test\n");
    printf("\n");

    ws2812.init();

    constexpr uint8_t brightness = 0xff;
    constexpr int sleep_time_ms = 3000;
    while (true) {
        set(0x00, 0x00, 0x00);
        sleep_ms(sleep_time_ms);
        set(brightness, 0x00, 0x00);
        sleep_ms(sleep_time_ms);
        set(0x00, brightness, 0x00);
        sleep_ms(sleep_time_ms);
        set(0x00, 0x00, brightness);
        sleep_ms(sleep_time_ms);
        set(brightness, brightness, brightness);
        printf("min=%ldus max=%ldus avg=%ldus\n", set_min_us, set_max_us,
               (set_sum_us + set_cnt / 2) / set_cnt);
        sleep_ms(sleep_time_ms);
    }

} // int main()
