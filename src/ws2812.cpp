
#include <cstdint>
//
#include "hardware/pio.h"
//
#include "ws2812.h"
#include "ws2812.pio.h"


Ws2812::~Ws2812()
{
    pio_remove_program_and_unclaim_sm(&ws2812_program, _pio, _sm, _offset);
}


void Ws2812::init()
{
    // Some of the following looks at the clocks and I'm not sure what is
    // valid at constructor time, hence this extra "init" call

    bool status = pio_claim_free_sm_and_add_program_for_gpio_range(
        &ws2812_program, &_pio, &_sm, &_offset, _gpio, 1, true);
    assert(status);
    (void)status;

    ws2812_program_init(_pio, _sm, _offset, _gpio, 800000, false);

    set(0, 0, 0);
}


void Ws2812::set(uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t grb = (uint32_t(green) << 24) | (uint32_t(red) << 16) | (uint32_t(blue) << 8);

    // Writes to the pio in this loop are assumed to be fast enough so there's
    // no inter-pixel delay (50 usec). Also, the writes do not block unless
    // there are more than 8 LEDs ("count" parameter to constructor) or
    // multiple calls to set are made too quickly. Clocking out each LED takes
    // 30 usec, so, for example, if count = 8, a call less than 240 usec after
    // the previous call risks blocking.

    for (int i = 0; i < _count; i++)
        pio_sm_put_blocking(_pio, _sm, grb);
}
