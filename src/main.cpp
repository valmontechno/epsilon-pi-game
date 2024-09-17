#include "eadkpp.h"

using namespace EADK;

extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Hello world";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;

int main() {
  Display::pushRectUniform(Screen::Rect, 0xf6f6f6);
  Display::drawString("Hello world", Point(Screen::Width / 2 - 11 * 10 / 2, Screen::Height / 2 - 18 / 2), true, 0x414041, 0xf6f6f6);

  Keyboard::State keyState;

  while (true) {
    keyState = Keyboard::scan();

    // Let's code !

    if (keyState.keyDown(Keyboard::Key::Home) || keyState.keyDown(Keyboard::Key::Back)) return 0;
  }
}
