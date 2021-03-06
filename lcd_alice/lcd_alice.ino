#include <Timer.h>
#include "lcd_can.h"
#include "lcd.h"
#include "joystick.h"

Can myCan;
Lcd myLcd;
Joystick joyA(JOY_LEFT_1, JOY_RIGHT_1, JOY_UP_1, JOY_DOWN_1, JOY_PUSH_1);
Joystick joyB(JOY_LEFT_2, JOY_RIGHT_2, JOY_UP_2, JOY_DOWN_2, JOY_PUSH_2);

Timer displayTimer;
Timer indicatorTimer;
Timer rtcTimer;

void setup() {
  Serial.begin(9600);

  myCan.begin();
  myCan.send_wipers(false);
  myCan.send_horn(false);
  myCan.send_signals(false, false, false);
  myCan.send_headlights(false);
  joyA.begin();
  joyB.begin();
  myLcd.begin();
  myLcd.speed_dials();
  myLcd.logo();
	myLcd.draw_base_infoText();
  myLcd.time(0, 0, 0);

  myLcd.speed(0);
  myLcd.light(false);
  myLcd.hazards(false);
  myLcd.wiper(false);
  myLcd.highlight_button(LIGHT);

  displayTimer.reset();
  indicatorTimer.reset();
  rtcTimer.reset();
}

bool indicator_left_flag = false;
bool indicator_right_flag = false;
bool blinker = false;
int button = 1;
int light = false;
int wiper = false;
int hazards = false;
int speed = 0;
int fc_voltage = 0;
int fc_current = 0;
int m_current = 0;
unsigned int * now;

void loop() {
  myCan.read(); // read at max speed

  // INDICATOR STUFF
  // left indictor
  if(joyA.left->onPressed() && !hazards) {
    if(!indicator_left_flag) {
      // check if right indicator was on to turn it off
      indicator_right_flag = false;
      Serial.println("Right indictor: Off");
      myLcd.indictor(false, false);
      // turn on left indicator
      indicator_left_flag = true;
      Serial.println("Left indictor: On");
      blinker = true;
      myCan.send_signals(true, false, false);
    } else {
      // turn off indictor
      indicator_left_flag = false;
      Serial.println("Left indictor: Off");
      blinker = false;
      myLcd.indictor(false, false);
      myCan.send_signals(false, false, false);
    }
  }

  // right indicator
  if(joyA.right->onPressed() && !hazards) {
    if(!indicator_right_flag) {
      // check if left indicator was on to turn it off
      indicator_left_flag = false;
      Serial.println("Left indictor: Off");
      myLcd.indictor(false, false);
      // turn on indicator
      indicator_right_flag = true;
      Serial.println("Right indictor: On");
      blinker = true;
      myCan.send_signals(false, true, false);
    } else {
      // turn off indictor
      indicator_right_flag = false;
      Serial.println("Right indictor: Off");
      blinker = false;
      myLcd.indictor(false, false);
      myCan.send_signals(false, false, false);
    }
  }

  // indicator blinking
  if(indicatorTimer.elapsed(500)) {
    if(indicator_left_flag) {
      myLcd.indictor(blinker, false);
      blinker = !blinker;
    }
    if(indicator_right_flag) {
      myLcd.indictor(false, blinker);
      blinker = !blinker;
    }
    if(hazards) {
      myLcd.indictor(blinker, blinker);
      blinker = !blinker;
    }
  }

  // HORN STUFF
  if(joyA.push->onPressed()) {
    Serial.println("Horn On");
    myCan.send_horn(true);
  }
  if(joyA.push->onReleased()) {
    Serial.println("Horn Off");
    myCan.send_horn(false);
  }

  // NAVIGATION
  if(joyB.left->onPressed()) {
    button = (button - 1);
    if( button <= 0 ) button = 3;
    myLcd.highlight_button(button);
  }
  if(joyB.right->onPressed()) {
    button = (button + 1);
    if( button > 3 ) button = 1;
    myLcd.highlight_button(button);
  }
  if(joyB.push->onPressed()) {
    switch(button) {
    case LIGHT:
      if(light) {
        // if on turn it off
        Serial.println("Lights: Off");
        myLcd.light(false);
        myCan.send_headlights(false);
        light = false;
      } else {
        // turn on lights
        Serial.println("Lights: On");
        myLcd.light(true);
        myCan.send_headlights(true);
        light = true;
      }
      break;
    case WIPER:
      if(wiper) {
        // turn wiper off
        Serial.println("Wipers: Off");
        myLcd.wiper(false);
        myCan.send_wipers(false);
        wiper = false;
      } else {
        // turn wiper on
        Serial.println("Wipers: On");
        myLcd.wiper(true);
        myCan.send_wipers(true);
        wiper = true;
      }
      break;
    case HAZARDS:
      if(hazards) {
        // turn hazards off
        Serial.println("Hazards: Off");
        myLcd.hazards(false);
        myCan.send_signals(false, false, false);
        myLcd.indictor(false, false);
        hazards = false;
      } else {
        // turn hazards on
        Serial.println("Hazards: On");
        // turn off left and right off
        // check if left indicator was on to turn it off
        indicator_left_flag = false; indicator_right_flag = false;
        Serial.println("Left indictor: Off"); Serial.println("Right indictor: Off");
        myLcd.indictor(false, false);
        blinker = true;
        myLcd.hazards(true);
        myCan.send_signals(false, false, true);
        hazards = true;
      }
      break;
    }
  }

  if(displayTimer.elapsed(200)) {
    if(myCan.speed_available()) {
      speed = myCan.speed();
      Serial.print("Speed: ");
      Serial.println(speed);
      myLcd.speed(speed);
    }
		if(myCan.fc_voltage_available()) {
      fc_voltage = myCan.fc_voltage();
      Serial.print("FC Voltage: ");
      Serial.println(fc_voltage);
      myLcd.fc_voltage(fc_voltage);
    }
		if(myCan.fc_current_available()) {
      fc_current = myCan.fc_current();
      Serial.print("FC Current: ");
      Serial.println(fc_current);
      myLcd.fc_current(fc_current);
    }
		if(myCan.mCurrent_available()) {
      m_current = myCan.mCurrent();
      Serial.print("Motor Current: ");
      Serial.println(m_current);
      myLcd.motor_current(m_current);
    }
  }

  if(rtcTimer.elapsed(500)) {
    if(myCan.time_available()) {
      now = myCan.time();
      Serial.print("Time: ");
      Serial.print(now[0]); Serial.print(":");
      Serial.print(now[1]); Serial.print(":");
      Serial.println(now[2]);
      myLcd.time(now[0], now[1], now[2]);
    }
  }

}
