#include "sys/_stdint.h"
#include <appdef.h>
#include <sdk/calc/calc.h>
#include <sdk/os/debug.h>
#include <sdk/os/input.h>
#include <sdk/os/lcd.h>
#include <stdio.h>
#include <stdlib.h>

APP_NAME("Doodlepad")
APP_DESCRIPTION("A drawing app.")
APP_AUTHOR("Decky Boiii")
APP_VERSION("2.0.0")

/*
 * Fill this section in with some information about your app.
 * All fields are optional - so if you don't need one, take it out.
 */

    {
        for (int j = 0; j < 4; j++)
        {
            setPixel(pen_slider_bar_x + j, i, RGB_TO_RGB565(0, 0, 0));
        }
    }

    // Display pen thickness
    if (show_pen_thickness)
    {
        Debug_Printf(0, 0, false, 0, "Pen Thickness: %d", pen_thickness);
    }
  }

  // Display pen thickness
  if (show_pen_thickness) {
    Debug_Printf(0, 0, false, 0, "Pen Thickness: %d", static_cast<int>(pen_thickness));
  }
}

void drawHud() { drawHud(false); }

int main() {
  struct Input_Event event __attribute__((aligned(4)));
  unsigned int screen_width_u, screen_height_u;
  LCD_GetSize(&screen_width_u, &screen_height_u);
  screen_width = screen_width_u;
  screen_height = screen_height_u;

    LCD_ClearScreen();
    drawHud();

    LCD_Refresh();

  int old_p1_x, old_p1_y;

    old_p1_x = 0;
    old_p1_y = 0;

  while (true) {
    if (GetInput(&event, 0xFFFFFFFF, 0x10) < 0) {
      continue;
    }

    if (event.type == EVENT_KEY && event.data.key.direction == KEY_PRESSED) {
      if (event.data.key.keyCode == KEYCODE_POWER_CLEAR) {
        break;
      }
      if (event.data.key.keyCode == KEYCODE_BACKSPACE) {
        LCD_ClearScreen();
        drawHud();
        LCD_Refresh();
      }
    }

    switch (event.type) {
    case EVENT_TOUCH:
      if (event.data.touch_single.direction == TOUCH_DOWN) {
        double radius = static_cast<double>(pen_thickness) / 2;
        for (int x = -radius; x <= radius; x++) {
          for (int y = -radius; y <= radius; y++) {
            if (x * x + y * y <= radius * radius) {
              setPixel(event.data.touch_single.p1_x + x,
                       event.data.touch_single.p1_y + y, active_colour);
            }

            if (event.data.touch_single.direction == TOUCH_HOLD_DRAG)
            {
                double radius = static_cast<double>(pen_thickness) / 2;
                for (int x = -radius; x <= radius; x++)
                {
                    for (int y = -radius; y <= radius; y++)
                    {
                        if (x * x + y * y <= radius * radius)
                        {
                            line(old_p1_x + x, old_p1_y + y, event.data.touch_single.p1_x + x,
                                 event.data.touch_single.p1_y + y, active_colour);
                        }
                    }
                }
                old_p1_x = event.data.touch_single.p1_x;
                old_p1_y = event.data.touch_single.p1_y;

                if (event.data.touch_single.p1_y >= 500)
                {
                    float pos = event.data.touch_single.p1_x - 2;
                    if (pos > 316)
                        pos = 316;
                    // 30 is max pen thickness
                    pen_thickness = floor(pos * (30) / 316) + 1;
                    drawHud(true);
                }
                else
                {
                    drawHud();
                }
            }

      LCD_Refresh();
      break;
    default:
      break;
    }

  return 0;
}