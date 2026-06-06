
#include <cstdio>
//
#include "pico/stdio.h"
#include "pico/stdio_usb.h"
#include "pico/stdlib.h"
#include "pico/time.h" // sleep_ms
//
#include "ws2812/ws2812.h"

// PIO will use this gpio to drive LEDs
static constexpr int ws2812_gpio = 12;

// how many LEDs on the string attached to ws2812_gpio
static constexpr int str_len = 4;

static Ws2812 ws2812(ws2812_gpio, str_len);

// Each call to set() should be very quick as long as the
// number of LEDs <= FIFO depth (8 for 2040 and 2350).
// Keep track of min/max/avg call time to verify.
static int32_t set_min_us = INT32_MAX;
static int32_t set_max_us = INT32_MAX;
static int32_t set_sum_us = 0;
static int32_t set_cnt = 0;


// Set the LEDs, timing how long the set() call takes.
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

// clang-format off
static void off()           { set(0,   0,   0);   }
static void red(int brt)    { set(brt, 0,   0);   }
static void green(int brt)  { set(0,   brt, 0);   }
static void blue(int brt)   { set(0,   0,   brt); }
static void yellow(int brt) { set(brt, brt, 0);   }
static void purple(int brt) { set(brt, 0,   brt); }
static void cyan(int brt)   { set(0,   brt, brt); }
static void white(int brt)  { set(brt, brt, brt); }
// clang-format on


int main()
{
    stdio_init_all();

#if 0
    while (!stdio_usb_connected())
        tight_loop_contents();
#endif

    sleep_ms(10);

    printf("\n");
    printf("ws2812_test\n");
    printf("\n");

    ws2812.init();

    constexpr uint8_t brt = 0xff;
    constexpr int sleep_time_ms = 3000;
    while (true) {
        off();
        sleep_ms(sleep_time_ms);
        red(brt);
        sleep_ms(sleep_time_ms);
        green(brt);
        sleep_ms(sleep_time_ms);
        blue(brt);
        sleep_ms(sleep_time_ms);
        yellow(brt);
        sleep_ms(sleep_time_ms);
        purple(brt);
        sleep_ms(sleep_time_ms);
        cyan(brt);
        sleep_ms(sleep_time_ms);
        white(brt);
        printf("min=%ldus max=%ldus avg=%ldus\n", set_min_us, set_max_us,
               (set_sum_us + set_cnt / 2) / set_cnt);
        sleep_ms(sleep_time_ms);
    }

} // int main()
