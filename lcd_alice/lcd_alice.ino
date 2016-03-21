#include <Timer.h>
#include "lcd_can.h"

Can myCan;
Timer displayTimer;

void setup() {
  Serial.begin(9600);
  myCan.begin();
  displayTimer.reset();
}

void loop() {
  myCan.read(); // read at max speed

  if(displayTimer.elapsed(50)) {
    if(myCan.speed_available())
      Serial.println(myCan.speed());
  }
}
