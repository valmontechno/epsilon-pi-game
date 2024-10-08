#include "eadkpp.h"
#include "pi_decimals.h"
#include "palette.h"
#include "font.h"
#include "font_characters.h"

using namespace EADK;

extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Pi";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;

constexpr uint8_t fieldLength = 13;
constexpr uint8_t fieldXOffset = (Screen::Width - (fieldLength * charWidth)) / 2;
constexpr uint8_t fieldYOffset = (Screen::Height - charHeight) / 2;

constexpr uint8_t scoreLength = 11;

Keyboard::State keyState;

char buffer[fieldLength];

void printBuffer(ColorRGB textColor=textColorRGB)
{
  for (uint8_t i = 0; i < fieldLength; i++)
  {
    printChar(buffer[i], fieldXOffset + i * charWidth, fieldYOffset, textColor, fieldColorRGB);
  }
}

void printBufferEmphasized(uint16_t digits, ColorRGB textColor, ColorRGB lastCharColor)
{
  for (uint8_t i = 0; i < fieldLength; i++)
  {
    ColorRGB color = digits < fieldLength ?
      (i + 1 == digits ? lastCharColor : textColor) :
      (i + 1 == fieldLength ? lastCharColor : textColor);
    printChar(buffer[i], fieldXOffset + i * charWidth, fieldYOffset, color, fieldColorRGB);
  }
}

void printBlanck()
{
  Display::pushRectUniform(Rect(fieldXOffset, fieldYOffset, fieldLength * charWidth, charHeight), fieldColorRGB);
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
    if (keyPress(Key::Back)) return ' ';
  }
}

bool waitForEXE()
{
  using Keyboard::Key;
  do
  {
    keyState = Keyboard::scan();
  } while (keyPress(Key::EXE) || keyPress(Key::OK) || keyPress(Key::Back) || keyPress(Key::Home));

  while (true)
  {
    keyState = Keyboard::scan();
    if (keyPress(Key::Back) || keyPress(Key::Home))
    {
      return true;
    }
    if (keyPress(Key::EXE) || keyPress(Key::OK))
    {
      return false;
    }
  }
}



void printScore(uint16_t n, uint16_t m)
{
  m -= 1;
  if (n > 1)
  {
    n -= 1;
  }

  char scoreBuffer[scoreLength + 1];
  char *c = scoreBuffer + scoreLength;

  *c = 0;

  do
  {
    c--;
    *c = '0' + m % 10;
    m /= 10;
  } while (m > 0);

  c--;
  *c = '/';

  do
  {
    c--;
    *c = '0' + n % 10;
    n /= 10;
  } while (n > 0);

  while (c > scoreBuffer)
  {
    c--;
    *c = ' ';
  }

  Display::drawString(scoreBuffer, Point(fieldXOffset + fieldLength * charWidth - scoreLength * 7, fieldYOffset - 5 - 18), false, borderColorRGB, bgColorRGB);
}

void printScoreBlanck()
{
  Display::pushRectUniform(Rect(fieldXOffset + fieldLength * charWidth - scoreLength * 7, fieldYOffset - 5 - 18, scoreLength * 7, 14), bgColorRGB);
}

void game()
{
  uint16_t digitProgress = 4;

  while (true)
  {
    // spell pi
    for (uint16_t i = 1; i <= digitProgress; i++)
    {
      writePi(i);
      printBuffer();
      printScore(i, digitProgress);
      Timing::msleep(i > digitProgress - fieldLength ? 300 : 100);
    }
    Timing::msleep(1000);

    printBlanck();
    printScore(0, digitProgress);

    for (uint16_t i = 0; i < digitProgress; i++)
    {
      char input = waitForInput();
      if (input == ' ')
      {
        return;
      }
      else if (input == pi[i])
      {
        // right digit
        uint16_t digits = i + 1;
        writePi(digits);
        printBuffer();
        printScore(digits, digitProgress);
      }
      else
      {
        // game over
        uint16_t digits = i + 1;
        writePi(digits);

        buffer[(digits < fieldLength ? digits : fieldLength) - 1] = input;
        printBufferEmphasized(digits, textColorRGB, errorColorRGB);
        Timing::msleep(200);
        printBufferEmphasized(digits, textColorRGB, fieldColorRGB);
        Timing::msleep(200);
        printBufferEmphasized(digits, textColorRGB, errorColorRGB);
        Timing::msleep(500);

        buffer[(digits < fieldLength ? digits : fieldLength) - 1] = pi[i];
        printBufferEmphasized(digits, textColorRGB, correctionColorRGB);
        waitForEXE();

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

    if (digitProgress > piDigits)
    {
      Display::drawString("GG - by Valmontechno", Point((Screen::Width - 10 * 20) / 2, (Screen::Height - 18) / 2), true, rightColorRGB, fieldColorRGB);
      waitForEXE();
      return;
    }
    

  }
}

int main()
{
  Display::pushRectUniform(Screen::Rect, bgColorRGB);
  Display::pushRectUniform(Rect(fieldXOffset - 5, fieldYOffset - 5, fieldLength * charWidth + 10, charHeight + 10), borderColorRGB);
  Display::pushRectUniform(Rect(fieldXOffset - 4, fieldYOffset - 4, fieldLength * charWidth + 8, charHeight +8), fieldColorRGB);

  while (true)
  {
    cleanBuffer();
    buffer[0] = 'p';
    buffer[1] = 'i';
    printBuffer();
    Display::drawString("Press EXE", Point((Screen::Width - 10 * 9) / 2, (Screen::Height - 18) / 2), true, textColorRGB, fieldColorRGB);

    if (waitForEXE())
    {
      return 0;
    }

    game();

    printScoreBlanck();
  }
}
