#pragma once

#include <climits>
#include <cstdint>
//
#include "hardware/pio.h"
#include "pico/stdlib.h"


class Ws2812
{

public:

    Ws2812(int gpio, int count=1) :
        _gpio(gpio),
        _count(count),
        _pio(nullptr),
        _sm(UINT_MAX),
        _offset(UINT_MAX)
    {
    }

    ~Ws2812();

    void init();

    void set(uint8_t red, uint8_t green, uint8_t blue);

private:

    int _gpio;

    int _count; // how many leds in the string (all will be set the same)

    PIO _pio; // pointer to the hardware registers

    uint _sm;

    uint _offset;
};
