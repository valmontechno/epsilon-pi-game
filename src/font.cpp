#include "font.h"
#include "eadkpp.h"
#include "font_characters.h"

using namespace EADK;

Color pixels[charLength];

void printChar(char chr, Color color, uint16_t x, uint16_t y)
{
    const uint8_t* chrPtr = getChar(chr);
    for (uint16_t i = 0; i < charLength; i++)
    {
        pixels[i] = chrPtr[i];
    }
    Display::pushRect(Rect(x, y, charWidth, charHeight), pixels);
}