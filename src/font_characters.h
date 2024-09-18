#ifndef FONT_CHARACTERS_H
#define FONT_CHARACTERS_H

#include "eadkpp.h"

constexpr uint8_t charWidth = 20;
constexpr uint8_t charHeight = 28;
constexpr uint16_t charLength = charWidth * charHeight;

const uint8_t* getChar(char chr);

#endif