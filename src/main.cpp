#include "sys/_stdint.h"
#include <appdef.hpp>
#include <sdk/calc/calc.hpp>
#include <sdk/os/debug.hpp>
#include <sdk/os/input.hpp>
#include <sdk/os/lcd.hpp>
#include <stdio.h>
#include <stdlib.h>

/*
 * Fill this section in with some information about your app.
 * All fields are optional - so if you don't need one, take it out.
 */
APP_NAME("Doodlepad")
APP_DESCRIPTION("A drawing app")
APP_AUTHOR("Decky Boiii")
APP_VERSION("1.0.1")

int floor(double x) {
  if (x >= 0) {
    return static_cast<int>(x); // Truncate towards zero for positive numbers
  } else {
    int truncated =
        static_cast<int>(x); // Truncate towards zero for negative numbers
    return (x == truncated) ? truncated : truncated - 1;
  }
}

int screen_width, screen_height;
uint32_t colours[8] = {
    RGB_TO_RGB565(0, 0, 0),   RGB_TO_RGB565(31, 0, 0),
    RGB_TO_RGB565(0, 63, 0),  RGB_TO_RGB565(0, 0, 31),
    RGB_TO_RGB565(31, 63, 0), RGB_TO_RGB565(0, 63, 31),
    RGB_TO_RGB565(31, 0, 31), RGB_TO_RGB565(31, 63, 31),
};
int colours_length = static_cast<int>(sizeof(colours) / sizeof(colours[0]));
uint32_t active_colour = colours[0];
int pen_thickness = 8;

bool menu_active = false;
uint16_t *drawing_cache_vram;

char *active_touch_type;

void drawHud(bool show_pen_thickness) {
  for (int i = 0; i <= colours_length - 1; i++) {
    for (int j = 0; j <= screen_width / colours_length; j++) {
      for (int k = 0; k <= 24; k++) { // 24 is the height of the colour tray
        setPixel(j + (i * (screen_width / colours_length)), k, colours[i]);
      }
    }
  }

  for (int i = 1; i <= 5; i++) {
    line(0, 24 + i, screen_width, 24 + i, active_colour);
  }
  line(0, 24 + 6, screen_width, 24 + 6, RGB_TO_RGB565(31, 63, 31));

  for (int i = 500; i <= 528; i++) {
    for (int j = 0; j < screen_width; j++) {
      setPixel(j, i, RGB_TO_RGB565(31, 63, 31));
    }
  }
  line(0, 500, 320, 500, RGB_TO_RGB565(0, 0, 0));

  int pen_slider_bar_x = floor(316.00 / (30 - 1) * (pen_thickness - 1));
  for (int i = 500; i <= 528; i++) {
    for (int j = 0; j < 4; j++) {
      setPixel(pen_slider_bar_x + j, i, RGB_TO_RGB565(0, 0, 0));
    }
  }

  // Display pen thickness
  if (show_pen_thickness) {
    Debug_Printf(0, 0, false, 0, "Pen Thickness: %d", pen_thickness);
  }
}

void drawHud() { drawHud(false); }

extern "C" void main() {
  calcInit(); // backup screen and init some variables
  LCD_GetSize(&screen_width, &screen_height);

  LCD_ClearScreen();
  drawHud();

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
      drawHud();
      LCD_Refresh();
    }

    GetInput(&event, 0xFFFFFFFF, 0x10);

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
          }
        }

        old_p1_x = event.data.touch_single.p1_x;
        old_p1_y = event.data.touch_single.p1_y;

        // Check if we are in the HUD zone
        if (event.data.touch_single.p1_y <= 24) {
          active_colour = colours[floor(
              event.data.touch_single.p1_x /
              (static_cast<float>(screen_width) / colours_length))];
          drawHud();
        } else if (event.data.touch_single.p1_y >= 500) {
          float pos = event.data.touch_single.p1_x - 2;
          if (pos > 316)
            pos = 316;
          // 30 is max pen thickness
          pen_thickness = floor(pos * (30) / 316) + 1;
          drawHud(true);
        } else {
          drawHud();
        }
      }

      if (event.data.touch_single.direction == TOUCH_HOLD_DRAG) {
        double radius = static_cast<double>(pen_thickness) / 2;
        for (int x = -radius; x <= radius; x++) {
          for (int y = -radius; y <= radius; y++) {
            if (x * x + y * y <= radius * radius) {
              line(old_p1_x + x, old_p1_y + y, event.data.touch_single.p1_x + x,
                   event.data.touch_single.p1_y + y, active_colour);
            }
          }
        }
        old_p1_x = event.data.touch_single.p1_x;
        old_p1_y = event.data.touch_single.p1_y;

        if (event.data.touch_single.p1_y >= 500) {
          float pos = event.data.touch_single.p1_x - 2;
          if (pos > 316)
            pos = 316;
          // 30 is max pen thickness
          pen_thickness = floor(pos * (30) / 316) + 1;
          drawHud(true);
        } else {
          drawHud();
        }
      }

      LCD_Refresh();
      break;
    }
  }

  calcEnd(); // restore screen and do stuff
}