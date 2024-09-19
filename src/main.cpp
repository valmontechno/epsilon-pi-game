#include "eadkpp.h"
#include "palette.h"
#include "font.h"
#include "font_characters.h"

using namespace EADK;

extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Pi";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;

constexpr uint16_t piDigits = 23 - 1;
const char* pi = "3.14159265358979323846";

constexpr uint8_t fieldLength = 13;
constexpr uint8_t fieldOffset = Screen::Width / 2 - (fieldLength * charWidth) / 2;

Keyboard::State keyState;

uint8_t buffer[fieldLength];

void printBuffer(ColorRGB textColor=textColorRGB)
{
  for (uint8_t i = 0; i < fieldLength; i++)
  {
    printChar(buffer[i], fieldOffset + i * charWidth, 100, textColor, bgColorRGB);
  }
}

void printBufferEmphasized(uint16_t digits, ColorRGB textColor, ColorRGB lastCharColor)
{
  for (uint8_t i = 0; i < fieldLength; i++)
  {
    ColorRGB color = digits < fieldLength ?
      (i + 1 == digits ? lastCharColor : textColor) :
      (i + 1 == fieldLength ? lastCharColor : textColor);
    printChar(buffer[i], fieldOffset + i * charWidth, 100, color, bgColorRGB);
  }
}

void printBlanck()
{
  Display::pushRectUniform(Rect(fieldOffset, 100, fieldLength * charWidth, charHeight), bgColorRGB);
}

void cleanBuffer()
{
  for (uint8_t i = 0; i < fieldLength; i++)
  {
    buffer[i] = ' ';
  }
}

void writePi(uint16_t digits)
{
  if (digits < fieldLength)
  {
    cleanBuffer();
    for (uint16_t i = 0; i < digits; i++)
    {
      buffer[i] = pi[i];
    }
  }
  else
  {
    uint16_t offset = digits - fieldLength;
    for (uint16_t i = 0; i < fieldLength; i++)
    {
      buffer[i] = pi[i + offset];
    }
  }
}

bool keyPress(Keyboard::Key key)
{
  if (keyState.keyDown(key))
  {
    do
    {
      keyState = Keyboard::scan();
    } while (keyState.keyDown(key));
    return true;
  }
  return false;
}

char waitForInput()
{
  using Keyboard::Key;
  while (true)
  {
    keyState = Keyboard::scan();
    if (keyPress(Key::Zero)) return '0';
    if (keyPress(Key::One)) return '1';
    if (keyPress(Key::Two)) return '2';
    if (keyPress(Key::Three)) return '3';
    if (keyPress(Key::Four)) return '4';
    if (keyPress(Key::Five)) return '5';
    if (keyPress(Key::Six)) return '6';
    if (keyPress(Key::Seven)) return '7';
    if (keyPress(Key::Eight)) return '8';
    if (keyPress(Key::Nine)) return '9';
    if (keyPress(Key::Dot)) return '.';
  }
}

void game()
{
  uint16_t digitProgress = 13;

  while (true)
  {
    for (uint16_t i = 1; i <= digitProgress; i++)
    {
      writePi(i);
      printBuffer();
      Timing::msleep(400);
    }
    Timing::msleep(1000);
    printBlanck();

    for (uint16_t i = 0; i < digitProgress; i++)
    {
      char input = waitForInput();
      if (input == pi[i])
      {
        // right digit
        writePi(i + 1);
        printBuffer();
      }
      else
      {
        // game over
        uint16_t digits = i + 1;
        writePi(i + 1);
        buffer[(digits < fieldLength ? digits : fieldLength) - 1] = input;
        printBufferEmphasized(digits, textColorRGB, errorColorRGB);
        Timing::msleep(1000);
        buffer[(digits < fieldLength ? digits : fieldLength) - 1] = pi[i];
        printBufferEmphasized(digits, textColorRGB, correctionColorRGB);
        Timing::msleep(1000);
        return;
      }

      if (keyState.keyDown(Keyboard::Key::Back))
      {
        return;
      }
    }

    // right sequence
    for (uint8_t i = 0; i < 2; i++)
    {
      Timing::msleep(100);
      printBuffer(rightColorRGB);
      Timing::msleep(100);
      printBlanck();
    }
    Timing::msleep(1000);

    digitProgress++;

  }
}

int main()
{
  Display::pushRectUniform(Screen::Rect, bgColorRGB);

  game();

  Display::pushRectUniform(Screen::Rect, errorColorRGB);

  while (true) {
    Timing::msleep(100);
    keyState = Keyboard::scan();
    if (keyState.keyDown(Keyboard::Key::Home) || keyState.keyDown(Keyboard::Key::Back)) return 0;
  }
}
