#include "lcd_can.h"

Can myCan;

void setup() {
  Serial.begin(9600);
  myCan.begin();
}

void loop() {
  myCan.read(); // read at max speed

  Serial.println(myCan.speed());
}
