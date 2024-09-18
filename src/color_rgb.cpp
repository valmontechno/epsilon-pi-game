#include "color_rgb.h"
#include "eadkpp.h"

ColorRGB::operator Color()
{
    return Color(r, g, b);
}

ColorRGB ColorRGB::Interpolate(ColorRGB color1, ColorRGB color2, uint8_t factor)
{
    uint16_t f = factor;
    uint16_t f_ = 255 - f;

    uint8_t r = ((uint16_t)color1.r * f + (uint16_t)color2.r * f_) / 255;
    uint8_t g = ((uint16_t)color1.g * f + (uint16_t)color2.g * f_) / 255;
    uint8_t b = ((uint16_t)color1.b * f + (uint16_t)color2.b * f_) / 255;

    return ColorRGB(r, g, b);
}