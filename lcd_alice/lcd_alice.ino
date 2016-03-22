#include <Timer.h>
#include "lcd_can.h"
#include "lcd.h"

Can myCan;
Lcd myLcd;
Timer displayTimer;
Timer indictorTimer;

void setup() {
  Serial.begin(9600);

  myCan.begin();
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
  indictorTimer.reset();
}

bool indictor = false;
int a = 1;
int b = 0;
void loop() {
  myCan.read(); // read at max speed

  if(displayTimer.elapsed(200)) {
    myLcd.speed(b);
    b++;
    if(b > 60) b = 0;
    if(myCan.speed_available()) {
      //Serial.println(a);
    }
  }

  if(indictorTimer.elapsed(500)) {
    myLcd.indictor(indictor, indictor);
    myLcd.highlight_button(a);
    a++;
    if(a > 3) a = 1;

    indictor = !indictor;
  }
}
