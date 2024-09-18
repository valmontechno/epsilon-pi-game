#ifndef COLOR_RGB_H
#define COLOR_RGB_H

#include "eadkpp.h"

using EADK::Color;

struct ColorRGB
{
    uint8_t r, g, b;

    constexpr ColorRGB() : r(0), g(0), b(0) {}
    constexpr ColorRGB(uint32_t rgb) : r((rgb&0xff0000)>>16), g((rgb&0x00ff00)>>8), b(rgb&0x0000ff) {}
    constexpr ColorRGB(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

    operator Color();

    static ColorRGB Interpolate(ColorRGB color1, ColorRGB color2, uint8_t factor);
};


#endif