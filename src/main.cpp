#include "eadkpp.h"
#include "pi_decimals.h"
#include "palette.h"
#include "font.h"
#include "font_characters.h"

using namespace EADK;

extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Pi";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;

constexpr uint16_t largeFontWidth = 10, largeFontHeight = 18;
constexpr uint16_t smallFontWidth = 7, smallFontHeight = 14;

constexpr uint8_t fieldLength = 13;
constexpr uint8_t fieldXOffset = (Screen::Width - (fieldLength * charWidth)) / 2;
constexpr uint8_t fieldYOffset = (Screen::Height - charHeight) / 2;

constexpr uint8_t scoreLength = 11;

constexpr uint16_t startupProgress = 4;

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
    if (keyPress(Key::Back)) return '\e';
    if (keyPress(Key::EXE) || keyPress(Key::OK)) return '\n';
  }
}

bool waitForConfirm()
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
  if (n > 1)
  {
    n -= 1;
  }
  if (m > 1){
    m -= 1;
  }

  char scoreBuffer[scoreLength + 1];
  char *c = scoreBuffer + scoreLength;

  *c = 0;

  if (m > 0)
  {
    do
    {
      c--;
      *c = '0' + m % 10;
      m /= 10;
    } while (m > 0);

    c--;
    *c = '/';
  }

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

  Display::drawString(scoreBuffer, Point(fieldXOffset + fieldLength * charWidth - scoreLength * smallFontWidth, fieldYOffset - 5 - 4 - smallFontHeight), false, borderColorRGB, bgColorRGB);
}

void printScoreBlanck()
{
  Display::pushRectUniform(Rect(fieldXOffset + fieldLength * charWidth - scoreLength * smallFontWidth, fieldYOffset - 5 - 4 - smallFontHeight, scoreLength * smallFontWidth, smallFontHeight), bgColorRGB);
}

void printLabelBlanck(){
  Display::pushRectUniform(Rect((Screen::Width - smallFontWidth * 26) / 2, 150, smallFontWidth * 26, smallFontHeight), bgColorRGB);
}

void spellPi(uint16_t digits){
  for (uint16_t i = 1; i <= digits; i++)
    {
      writePi(i);
      printBuffer();
      printScore(i, digits);
      Timing::msleep(i > digits - 4 ? 300 : 100);
    }
    Timing::msleep(1000);
}

void gameOver(uint16_t i, char input){
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
  waitForConfirm();
}

void gameWon(){
  printBlanck();
  Display::drawString("GG - by Valmontechno", Point((Screen::Width - largeFontWidth * 20) / 2, (Screen::Height - largeFontHeight) / 2), true, rightColorRGB, fieldColorRGB);
  waitForConfirm();
}

void game(uint16_t digitProgress)
{
  while (true)
  {
    // spell pi
    spellPi(digitProgress);

    printBlanck();
    printScore(0, digitProgress);

    for (uint16_t i = 0; i < digitProgress; i++)
    {
      char input = waitForInput();
      if (input == '\n')
      {
        i--;
      }
      else if (input == '\e')
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
        gameOver(i, input);
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
      gameWon();
      return;
    }

  }
}

int main()
{
  Display::pushRectUniform(Screen::Rect, bgColorRGB);
  Display::pushRectUniform(Rect(fieldXOffset - 5, fieldYOffset - 5, fieldLength * charWidth + 10, charHeight + 10), borderColorRGB);
  Display::pushRectUniform(Rect(fieldXOffset - 4, fieldYOffset - 4, fieldLength * charWidth + 8, charHeight +8), fieldColorRGB);

  do
  {
    keyState = Keyboard::scan();
  } while (keyPress(Keyboard::Key::EXE) || keyPress(Keyboard::Key::OK));

  while (true)
  {
    cleanBuffer();
    buffer[0] = 'p';
    buffer[1] = 'i';
    printBuffer();
    Display::drawString("Press EXE", Point((Screen::Width - largeFontWidth * 9) / 2, (Screen::Height - largeFontHeight) / 2), true, textColorRGB, fieldColorRGB);
    Display::drawString("or type 3.14 and press EXE", Point((Screen::Width - smallFontWidth * 26) / 2, 150), false, borderColorRGB, bgColorRGB);

    for (uint16_t i = 0; i < piDigits; i++)
    {
      uint16_t digits = i + 1;
      char input = waitForInput();
      if (input == '\n')
      {
        printLabelBlanck();
        game(digits > startupProgress ? digits : startupProgress);
        break;
      }
      else if (input == '\e')
      {
        return 0; // exit
      }
      else if (input == pi[i])
      {
        // right digit
        writePi(digits);
        printBuffer();
        printScore(digits, 0);

        if (digits == piDigits){
        printLabelBlanck();
          gameWon();
          break;
        }
      }
      else
      {
        // game over
        printLabelBlanck();
        gameOver(i, input);
        break;
      }
    }

    printScoreBlanck();

  }
}
