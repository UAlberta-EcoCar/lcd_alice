#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>
#include <Timer.h>
#include "lcd_can.h"
#include "colors.h"
#include "images.h"
#include "speedometer.h"

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

  tft.fillScreen(COLOR_BACKGROUND);

  // speedometer
  tft.drawCircle(507, 318, 250, COLOR_DIAL);
  tft.drawCircle(507, 318, 251, COLOR_DIAL);
  tft.drawCircle(507, 318, 252, COLOR_DIAL);
  // delete extra
  tft.fillRect(200, 318, 600, 162, COLOR_BACKGROUND);
  tft.fillRect(507, 50, 293, 430, COLOR_BACKGROUND);
  // add lines
  for(int i=0; i<sizeof(dial_lines)/sizeof(int)/4; i++) {
    tft.drawLine(dial_lines[i*4], dial_lines[i*4+1], dial_lines[i*4+2], dial_lines[i*4+3], COLOR_DIAL);
  }
  for(int i=0; i<23; i++) {
    tft.drawLine(speed_lines[i*4], speed_lines[i*4+1], speed_lines[i*4+2], speed_lines[i*4+3], COLOR_SPEED);
  }
  // add font
  tft.drawChar(380, 210, '2', COLOR_FONT_SPEED, COLOR_BACKGROUND, 8);
  tft.drawChar(430, 210, '3', COLOR_FONT_SPEED, COLOR_BACKGROUND, 8);
  tft.drawBitmap(475, 245, kph_bmp, 37, 19, COLOR_FONT_SPEED);

  // clock
  tft.textMode();
  tft.textSetCursor(650, 20);
  tft.textColor(0xFFFF, COLOR_BACKGROUND);
  tft.textEnlarge(1);
  tft.textWrite("12:23:30");
  tft.graphicsMode();

  // buttons
  tft.fillCircle(260, 400, 50, COLOR_BUTTON_LIGHT);
  tft.drawCircle(260, 400, 56, COLOR_BUTTON_DARK);
  tft.drawCircle(260, 400, 55, COLOR_BUTTON_DARK);
  tft.drawCircle(260, 400, 54, COLOR_BUTTON_DARK);
  tft.fillCircle(400, 400, 50, COLOR_BUTTON_DARK);
  tft.fillCircle(540, 400, 50, COLOR_BUTTON_DARK);
  tft.drawBitmap(233, 375, light_bmp, 60, 49, COLOR_BUTTON_IMG);
  tft.drawBitmap(371, 370, hazards_bmp, 60, 53, COLOR_BUTTON_IMG);
  tft.drawBitmap(510, 385, wipers_bmp, 60, 31, COLOR_BUTTON_IMG);

  // left indictor
  tft.fillTriangle(30, 400, 75, 370, 75, 430, COLOR_INDICTORS);
  tft.fillRect(75, 382, 30, 35, COLOR_INDICTORS);

  // right indictor
  tft.fillTriangle(770, 400, 725, 370, 725, 430, COLOR_INDICTORS);
  tft.fillRect(695, 382, 30, 35, COLOR_INDICTORS);

  // logo
  tft.drawBitmap(31, 20, logo1_bmp, 24, 31, COLOR_LOGO_DARK);
  tft.drawBitmap(61, 20, logo2_bmp, 68, 31, COLOR_LOGO_LIGHT);
  tft.drawBitmap(132, 20, logo3_bmp, 68, 31, COLOR_LOGO_DARK);

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
