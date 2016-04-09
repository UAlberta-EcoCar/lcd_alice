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
  void send_horn(bool state);

  void read();
  bool speed_available();
  uint16_t speed();
  bool time_available();
  unsigned int * time();
	bool fc_voltage_available();
	uint32_t fc_voltage();
private:
  unsigned int _speed;
  bool _speed_flag;
  unsigned int _time[3];
  bool _time_flag;
	unsigned int _fc_voltage;
	bool _fc_voltage_flag;
};

#endif
