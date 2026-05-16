#pragma once

#include <climits>
#include <cstdint>
//
#include "hardware/pio.h"
#include "pico/stdlib.h"


class Ws2812
{

public:

    Ws2812(int gpio, int count = 1, int count_max = 8) :
        _gpio(gpio),
        _count(count),
        _count_max(count_max),
        _pio(nullptr),
        _sm(UINT_MAX),
        _offset(UINT_MAX)
    {
    }

    ~Ws2812();

    void init();

    int count() const
    {
        return _count;
    }

    void set_count(int count)
    {
        if (count < 0)
            count = 0;
        else if (count > _count_max)
            count = _count_max;
        _count = count;
    }

    void set(uint8_t red, uint8_t green, uint8_t blue);

    // clang-format off
    void off()                      { set(  0,   0,   0); }
    void red(uint8_t brt = 0xff)    { set(brt,   0,   0); }
    void green(uint8_t brt = 0xff)  { set(  0, brt,   0); }
    void blue(uint8_t brt = 0xff)   { set(  0,   0, brt); }
    void yellow(uint8_t brt = 0xff) { set(brt, brt,   0); }
    void purple(uint8_t brt = 0xff) { set(brt,   0, brt); }
    void cyan(uint8_t brt = 0xff)   { set(  0, brt, brt); }
    void white(uint8_t brt = 0xff)  { set(brt, brt, brt); }
    // clang-format on

private:

    int _gpio;

    int _count; // how many leds in the string to light up

    const int _count_max; // how many leds total in the string

    PIO _pio; // pointer to the hardware registers

    uint _sm;

    uint _offset;

    inline uint32_t rgb_to_grb(uint8_t r, uint8_t g, uint8_t b)
    {
        // +-------+-------+-------+-------+
        // |   g   |   r   |   b   |   0   |
        // +-------+-------+-------+-------+
        return (uint32_t(g) << 24) | (uint32_t(r) << 16) | (uint32_t(b) << 8);
    }
};
