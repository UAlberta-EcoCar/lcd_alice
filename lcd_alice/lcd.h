#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>
#include "hardware.h"
#include "speedometer.h"
#include "colors.h"
#include "images.h"

#define LIGHT 1
#define HAZARDS 2
#define WIPER 3

class Lcd {
public:
  Lcd() {};
  void begin();
  void logo();
  void time(int hours, int mins, int seconds);
  void speed_dials();
  void speed(int val);
  void indictor(bool left, bool right);
  void light(bool on);
  void wiper(bool on);
  void hazards(bool on);
  void highlight_button(int button);

private:
  void draw_speed(int start, int stop, bool on);

  Adafruit_RA8875 * _tft;
  char _time_str[8];
  int _prev_speed;
  bool _prev_left, _prev_right;
  int _prev_hightlight;

};

#endif
