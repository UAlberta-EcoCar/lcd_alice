#include <Timer.h>
#include "lcd_can.h"
#include "lcd.h"
#include "joystick.h"

Can myCan;
Lcd myLcd;
Joystick joyA(JOY_LEFT_1, JOY_RIGHT_1, JOY_UP_1, JOY_DOWN_1, JOY_PUSH_1);

Timer displayTimer;
Timer indicatorTimer;

void setup() {
  Serial.begin(9600);

  myCan.begin();
  joyA.begin();
  myLcd.begin();
  myLcd.speed_dials();
  myLcd.logo();
  myLcd.time(23, 48, 23);

  myLcd.speed(0);
  myLcd.light(false);
  myLcd.hazards(false);
  myLcd.wiper(false);
  myLcd.highlight_button(LIGHT);

  displayTimer.reset();
  indicatorTimer.reset();
}

bool indictor = false;
bool indicator_left_flag = false;
bool indicator_right_flag = false;
bool blinker = false;

int a = 1;
int b = 0;
void loop() {
  myCan.read(); // read at max speed

  // left indictor
  if(joyA.left->onPressed()) {
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
  if(joyA.right->onPressed()) {
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

  // handle Horn
  if(joyA.push->onPressed()) {
    Serial.println("Horn On");
    myCan.send_horn(true);
  }

  if(joyA.push->onReleased()) {
    Serial.println("Horn Off");
    myCan.send_horn(false);
  }

  if(displayTimer.elapsed(200)) {
    myLcd.speed(b);
    b++;
    if(b > 60) b = 0;
    if(myCan.speed_available()) {
      //Serial.println(a);
    }
  }

  if(indicatorTimer.elapsed(500)) {
    if(indicator_left_flag) {
      myLcd.indictor(blinker, false);
      blinker = !blinker;
    }
    if(indicator_right_flag) {
      myLcd.indictor(false, blinker);
      blinker = !blinker;
    }

  }
}
