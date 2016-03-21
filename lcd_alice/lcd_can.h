#ifndef LCD_CAN_H
#define LCD_CAN_H

#include <Arduino.h>
#include <mcp2515_lib.h>
#include <mcp2515_filters.h>
#include <can_message.h>
#include <can_message_def.h>
#include "hardware.h"

class Can {
public:
  Can() {};
  void begin();

  void send_wipers(bool wipers);
  void send_signals(bool left, bool right, bool hazards);
  void send_headlights(bool state);

  void read();
  bool speed_available();
  uint16_t speed();
private:
  unsigned int _speed;
  unsigned int _speed_flag;
};

#endif
