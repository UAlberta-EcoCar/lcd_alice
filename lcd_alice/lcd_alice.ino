#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>
#include <Timer.h>
#include "lcd_can.h"
#include "colors.h"
#include "images.h"

Can myCan;
Timer displayTimer;

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);




void setup() {
  Serial.begin(9600);

  // Initialise the display
  if (!tft.begin(RA8875_800x480)) {
    Serial.println("LCD Display Not Found!");
    while (1);
  }
  Serial.println("Initialized LCD Display");
  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);

  // With hardware accelleration this is instant
  tft.fillScreen(COLOR_BACKGROUND);
  tft.fillCircle(260, 400, 50, COLOR_BUTTON_LIGHT);
  tft.drawCircle(260, 400, 56, COLOR_BUTTON_DARK);
  tft.drawCircle(260, 400, 55, COLOR_BUTTON_DARK);
  tft.drawCircle(260, 400, 54, COLOR_BUTTON_DARK);
  tft.fillCircle(400, 400, 50, COLOR_BUTTON_DARK);
  tft.fillCircle(540, 400, 50, COLOR_BUTTON_DARK);

  tft.drawBitmap(233, 375, light_bmp, 60, 49, COLOR_BUTTON_IMG);
  tft.drawBitmap(371, 370, hazards_bmp, 60, 53, COLOR_BUTTON_IMG);
  tft.drawBitmap(510, 385, wipers_bmp, 60, 31, COLOR_BUTTON_IMG);


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
