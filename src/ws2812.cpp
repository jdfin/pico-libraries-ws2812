
#include <cstdint>
//
#include "hardware/pio.h"
//
#include "ws2812/ws2812.h"
#include "ws2812.pio.h"


Ws2812::~Ws2812()
{
    // It might be nice to turn off the LEDs first, but that would require a
    // delay after the write and removing the program, and mystery delays in a
    // destructor seem bad.
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
    // Writes to the pio in this loop are assumed to be fast enough so there's
    // no inter-pixel delay (50 usec). Also, the writes do not block unless
    // there are more than 8 LEDs ("count" setting) or multiple calls to set
    // are made too quickly. Clocking out each LED takes 30 usec, so, for
    // example, if count = 8, a call less than 240 usec after the previous
    // call risks blocking.

    const uint32_t grb = rgb_to_grb(red, green, blue);

    for (int i = 0; i < _count; i++)
        pio_sm_put_blocking(_pio, _sm, grb);

    // the rest are off
    for (int i = _count; i < _count_max; i++)
        pio_sm_put_blocking(_pio, _sm, 0);

} // Ws2812::set()
