#include "font.h"
#include "eadkpp.h"
#include "font_characters.h"
#include "color_rgb.h"

using namespace EADK;

Color pixels[charLength];

void printChar(char chr, uint16_t x, uint16_t y, ColorRGB textColor, ColorRGB bgColor)
{
    const uint8_t* chrPtr = getChar(chr);
    for (uint16_t i = 0; i < charLength; i++)
    {
        pixels[i] = ColorRGB::Interpolate(textColor, bgColor, chrPtr[i]);
    }
    Display::pushRect(Rect(x, y, charWidth, charHeight), pixels);
}