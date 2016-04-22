#include "lcd_can.h"

// CAN Message Definitions
//LCD Wipers
can_msg::MsgEncode wipers_msg( can_msg::BOOL, can_msg::AUX, can_msg::WIPERS, can_msg::IMPORTANT, 1);
//LCD Signals
can_msg::MsgEncode signals_msg( can_msg::BOOL, can_msg::AUX, can_msg::SIGNAL, can_msg::IMPORTANT, 3);
//LCD Headlight
can_msg::MsgEncode headlights_msg( can_msg::BOOL, can_msg::AUX, can_msg::HEADLIGHTS, can_msg::IMPORTANT, 1);
// Horn
can_msg::MsgEncode horn_msg( can_msg::BOOL, can_msg::AUX, can_msg::HORN, can_msg::IMPORTANT, 1);
//motor speed
can_msg::MsgEncode mspeed_msg( can_msg::UINT16, can_msg::MOTOR, can_msg::MCURRENT, can_msg::INFORMATION, 1 );
//motor current
can_msg::MsgEncode mcurrent_msg( can_msg::UINT16, can_msg::MOTOR, can_msg::MSPEED, can_msg::INFORMATION, 1 );
// rtc
can_msg::MsgEncode time_msg( can_msg::UINT8, can_msg::OTHER, can_msg::TIME, can_msg::INFORMATION, 6);
// fc voltage
can_msg::MsgEncode fc_voltage_msg( can_msg::INT32, can_msg::FUEL_CELL, can_msg::FC_CURR, can_msg::INFORMATION, 1);
// fc current
can_msg::MsgEncode fc_current_msg( can_msg::INT32, can_msg::FUEL_CELL, can_msg::FC_VOLT, can_msg::INFORMATION, 1);

/*
 * Initializes CAN bus
 */
void Can::begin() {
  // Initialize CAN
  Serial.println("Initializing CAN Controller");
  if (can_init(0,0,0,0,0,0,0,0)){
    Serial.println("Error: CAN initialization D:");
    while(1); // hang up program
  }
  Serial.println("CAN Controller Initialized :D");
}

/*
 * Send CAN wiper message
 */
void Can::send_wipers(bool wipers) {
  CanMessage message;
  message.id = wipers_msg.id();
	message.length = wipers_msg.len();
	wipers_msg.buf( message.data, wipers );
	can_send_message(&message);
}

/*
 * Send CAN signals message
 */
void Can::send_signals(bool left, bool right, bool hazards) {
  CanMessage message;
	message.id = signals_msg.id();
	message.length = signals_msg.len();
	signals_msg.buf( message.data, left << can_msg::LEFT_SIGNAL | right << can_msg::RIGHT_SIGNAL | hazards << can_msg::HAZARD_LIGHTS );
  can_send_message(&message);
}

/*
 * Send CAN headlights message
 */
void Can::send_headlights(bool state) {
  CanMessage message;
  message.id = headlights_msg.id();
	message.length = headlights_msg.len();
	headlights_msg.buf( message.data, state );
	can_send_message(&message);
}

void Can::send_horn(bool state) {
  CanMessage message;
  message.id = horn_msg.id();
	message.length = horn_msg.len();
	horn_msg.buf( message.data, state );
	can_send_message(&message);
}

/*
 * Read CAN Bus
 */
void Can::read() {
  if(!digitalRead(CAN_INT)) {
    CanMessage message = can_get_message();
    // read motor speed
    if( message.id == mspeed_msg.id()) {
      _speed = message.data[0] | (message.data[1] << 8);
      _speed_flag = true;
    }
    // read time
    if( message.id == time_msg.id() ) {
      _time[0] = message.data[can_msg::HOUR];
      _time[1] = message.data[can_msg::MINUTE];
      _time[2] = message.data[can_msg::SECOND];
      _time_flag = true;
    }
    // TODO: read motor current / voltage
		if( message.id == mcurrent_msg.id() ) {
      _mCurrent = message.data[0] | (message.data[1] << 8);
      _mCurrent_flag = true;
    }

    // TODO: read fuel cell voltage / current
		if(message.id == fc_voltage_msg.id()) {
      _fc_voltage = message.data[0] | (message.data[1] << 8);
      _fc_voltage_flag = true;
    }
		if(message.id == fc_current_msg.id()) {
      _fc_current = message.data[0] | (message.data[1] << 8);
      _fc_current_flag = true;
    }
  }
}

/*
 * Getters
 */
// Speed
bool Can::speed_available() { return _speed_flag; }
uint16_t Can::speed() {
  _speed_flag = false;
  return _speed;
}

// Time
bool Can::time_available() { return _time_flag; }
unsigned int * Can::time() {
  _time_flag = false;
  return _time;
}

// FC voltage
bool Can::fc_voltage_available() { return _fc_voltage_flag; }
uint32_t Can::fc_voltage() {
  _fc_voltage_flag = false;
  return _fc_voltage;
}

// FC current
bool Can::fc_current_available() { return _fc_current_flag; }
uint32_t Can::fc_current() {
  _fc_current_flag = false;
  return _fc_current;
}

// Motor Current
bool Can::mCurrent_available() { return _mCurrent_flag ; }
uint16_t Can::mCurrent() {
  _mCurrent_flag = false;
  return _mCurrent;
}
