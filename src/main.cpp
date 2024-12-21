#include <appdef.hpp>
#include <sdk/calc/calc.hpp>
#include <sdk/os/input.hpp>
#include <sdk/os/lcd.hpp>

/*
 * Fill this section in with some information about your app.
 * All fields are optional - so if you don't need one, take it out.
 */
APP_NAME("Doodlepad")
APP_DESCRIPTION("A drawing app")
APP_AUTHOR("Decky Boiii")
APP_VERSION("1.0.0")

extern "C" void main() {
  calcInit(); // backup screen and init some variables

  LCD_ClearScreen();
  LCD_Refresh();

  struct InputEvent event;
  int old_p1_x, old_p1_y;

  old_p1_x = 0;
  old_p1_y = 0;

  while (true) {

    uint32_t key1, key2;  // First create variables
    getKey(&key1, &key2); // then read the keys
    if (testKey(
            key1, key2,
            KEY_CLEAR)) { // Use testKey() to test if a specific key is pressed
      break;
    }
    if (testKey(key1, key2, KEY_BACKSPACE)) {
      LCD_ClearScreen();
      LCD_Refresh();
    }

    GetInput(&event, 0xFFFFFFFF, 0x10);

    switch (event.type) {
    case EVENT_TOUCH:
      if (event.data.touch_single.direction == TOUCH_DOWN) {
        setPixel(event.data.touch_single.p1_x, event.data.touch_single.p1_y,
                 RGB_TO_RGB565(0, 0, 0));
        old_p1_x = event.data.touch_single.p1_x;
        old_p1_y = event.data.touch_single.p1_y;
      }

      if (event.data.touch_single.direction == TOUCH_HOLD_DRAG) {
        line(old_p1_x, old_p1_y, event.data.touch_single.p1_x,
             event.data.touch_single.p1_y, RGB_TO_RGB565(0, 0, 0));
        old_p1_x = event.data.touch_single.p1_x;
        old_p1_y = event.data.touch_single.p1_y;
      }

      LCD_Refresh();
      break;
    }
  }

  calcEnd(); // restore screen and do stuff
}