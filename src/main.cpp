#include "eadkpp.h"
#include "palette.h"
#include "font.h"

using namespace EADK;

extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Pi";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;

const char* pi = "3.14159265358979323846";

Keyboard::State keyState;

char buffer[255];

void printBuffer(Color color)
{
  Display::drawString(buffer, Point(0, 0), true, color, bgColor);
}

void clearField()
{
  Display::drawString("                                        ", Point(0, 0), true, textColor, bgColor);
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

void spellPi(uint8_t digitProgress)
{
  for (uint8_t i = 1; i <= digitProgress; i++)
  {
    Timing::msleep(700);
    buffer[i] = pi[i];
    buffer[i + 1] = 0;
    printBuffer(textColor);
  }
}

bool game()
{
  uint8_t digitProgress = 3;

  while (true)
  {
    spellPi(digitProgress);
    Timing::msleep(1000);

    clearField();

    for (uint8_t i = 0; i <= digitProgress; i++)
    {
      if (waitForInput() == pi[i])
      {
        buffer[i] = pi[i];
        buffer[i + 1] = 0;
        printBuffer(textColor);
      }
      else
      {
        return false; // game over
      }

      if (keyState.keyDown(Keyboard::Key::Back))
      {
        return true;
      }
    }

    for (uint8_t i = 0; i < 2; i++)
    {
      printBuffer(rightColor);
      Timing::msleep(100);
      clearField();
      Timing::msleep(100);
    }

    digitProgress++;
  }
}

int main()
{
  Display::pushRectUniform(Screen::Rect, bgColor);

  // game();

  printChar('0', 20*0, 100, textColorRGB, bgColorRGB);
  printChar('1', 20*1, 100, textColorRGB, bgColorRGB);
  printChar('2', 20*2, 100, textColorRGB, bgColorRGB);
  printChar('3', 20*3, 100, textColorRGB, bgColorRGB);
  printChar('4', 20*4, 100, textColorRGB, bgColorRGB);
  printChar('5', 20*5, 100, textColorRGB, bgColorRGB);
  printChar('6', 20*6, 100, textColorRGB, bgColorRGB);
  printChar('7', 20*7, 100, textColorRGB, bgColorRGB);
  printChar('8', 20*8, 100, textColorRGB, bgColorRGB);
  printChar('9', 20*9, 100, textColorRGB, bgColorRGB);
  printChar('.', 20*10, 100, textColorRGB, bgColorRGB);

  Display::drawString("Game over", Point(0, 0), true, 0xff0000, bgColor);

  while (true) {

    // Let's code !

    Timing::msleep(100);

    keyState = Keyboard::scan();
    if (keyState.keyDown(Keyboard::Key::Home) || keyState.keyDown(Keyboard::Key::Back)) return 0;
  }
}
