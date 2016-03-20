#include "lcd_can.h"

// CAN Message Definitions
//LCD Wipers
can_msg::MsgEncode wipers_msg( can_msg::BOOL, can_msg::AUX, can_msg::WIPERS, can_msg::IMPORTANT, 1);
//LCD Signals
can_msg::MsgEncode signals_msg( can_msg::BOOL, can_msg::AUX, can_msg::SIGNAL, can_msg::IMPORTANT, 3);
//LCD Headlight
can_msg::MsgEncode headlights_msg( can_msg::BOOL, can_msg::AUX, can_msg::HEADLIGHTS, can_msg::IMPORTANT, 1);
//motor speed
can_msg::MsgEncode mspeed_msg( can_msg::UINT16, can_msg::MOTOR, can_msg::MSPEED, can_msg::INFORMATION, 1 );
/*
 * Initializes CAN bus
 */
void Can::begin() {
  // Initialize CAN
  Serial.println("Initializing CAN Controller");
  if (can_init(0,0,0,0,0,0,0,0)){
    Serial.println("Error: CAN initialization :(");
    while(1); // hang up program
  }
  Serial.println("CAN Controller Initialized :)");
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

/*
 * Read CAN Bus
 */
void Can::read() {
  if(!digitalRead(CAN_INT)) {
    CanMessage message = can_get_message();
    // read motor speed
    if( message.id == mspeed_msg.id()) _speed = message.data[0] | (message.data[1] << 8);
    // TODO: read time
    // TODO: read motor current / voltage
    // TODO: read fuel cell voltage / current
  }
}
