#include "mcp2515_lib.h"
#include "mcp2515_filters.h"
#include "can_message.h"
#include "can_message_def.h"

// Create speed msg
can_msg::MsgEncode speed_msg( can_msg::UINT16, can_msg::MOTOR, can_msg::MSPEED, can_msg::INFORMATION, 1 );
//LCD WIpers
can_msg::MsgEncode lcd_wipers_msg( can_msg::BOOL, can_msg::AUX, can_msg::WIPERS, can_msg::IMPORTANT, 1);
//LCD Signals
can_msg::MsgEncode lcd_signals_msg( can_msg::BOOL, can_msg::AUX, can_msg::SIGNAL, can_msg::IMPORTANT, 3);
//LCD Headlight
can_msg::MsgEncode lcd_headlights_msg( can_msg::BOOL, can_msg::AUX, can_msg::HEADLIGHTS, can_msg::IMPORTANT, 1);

unsigned int speed;

void setup() {
  Serial.begin(9600);
  // Initialize CAN
  Serial.print("Initializing mcp2515 CAN controller... ");
  if (can_init(0,0,0,0,0,0,0,0)){ //filter set to accept throttle and brake msg's
    Serial.println("mcp2515 config error");
    while(1); //hang up program
  }
  Serial.println("mcp2515 initialization successful");
  delay(1000);
}

unsigned int i = 0;
unsigned long previousMillis = 0;

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 100) {
    previousMillis = currentMillis;
    i++;
    if( i > 255 ) { i = 0; }
    send_speed(i);
  }
  CanMessage message; //make a new message object
  if(digitalRead(9) == 0) {
    message = can_get_message(); //retrieve message
    if( message.id == lcd_wipers_msg.id() ) {
      Serial.print("Wipers: ");
      Serial.println(message.data[0] ? "Yes" : "No");
    }
    if( message.id == lcd_signals_msg.id() ) {
      Serial.print("Signal: ");
      Serial.println(message.data[0]);
    }
    if( message.id == lcd_headlights_msg.id() ) {
      Serial.print("Headlights: ");
      Serial.println(message.data[0] ? "Yes" : "No");
    }
  }
}

void send_speed(unsigned int val) {
  //create new message
  CanMessage message_send;
  //set message id
  message_send.id = speed_msg.id();
  //set message length
  message_send.length = speed_msg.len();
  //set whether message is a remote transmit request (it's not)
  message_send.RTransR = 0;
  //set 8 data bytes
  speed_msg.buf(message_send.data, val); //16bit unsigned int

  can_send_message(&message_send);  
}
