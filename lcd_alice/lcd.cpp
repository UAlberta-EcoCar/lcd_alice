#include "lcd.h"
//#include <Fonts/FreeSans18pt7b.h>

void Lcd::begin() {
  _tft = new Adafruit_RA8875(RA8875_CS, RA8875_RESET);

  // Initialise the display
  if (!_tft->begin(RA8875_800x480)) {
    Serial.println("Error: LCD Display Not Found :(");
    while (1);
  }
  Serial.println("LCD Display Initialized :)");
  _tft->displayOn(true);
  _tft->GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  _tft->PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  _tft->PWM1out(255);

	//_tft->setFont(&FreeSans18pt7b);
  _tft->fillScreen(COLOR_BACKGROUND);

  _prev_speed = 0; _prev_left = false; _prev_right = false; _prev_hightlight = 4;
}

void Lcd::logo() {
  int x = 31; int y = 20;
  _tft->drawBitmap(x, y, logo1_bmp, 24, 31, COLOR_LOGO_DARK);
  _tft->drawBitmap(x+30, y, logo2_bmp, 68, 31, COLOR_LOGO_LIGHT);
  _tft->drawBitmap(x+100, y, logo3_bmp, 68, 31, COLOR_LOGO_DARK);
}

void Lcd::time(int hours, int mins, int secs) {
  int x = 650; int y = 20;
  _tft->textMode();
  _tft->textSetCursor(x, y);
  _tft->textColor(0xFFFF, COLOR_BACKGROUND);
  _tft->textEnlarge(1);
  sprintf(_time_str,"%.2d:%.2d:%.2d",hours,mins,secs);
  _tft->textWrite(_time_str);
  _tft->graphicsMode();
}

void Lcd::speed_dials() {
  // create dial
  // // draw circles
  _tft->drawCircle(507, 318, 250, COLOR_DIAL);
  _tft->drawCircle(507, 318, 251, COLOR_DIAL);
  _tft->drawCircle(507, 318, 252, COLOR_DIAL);
  // // delete extra
  _tft->fillRect(200, 318, 600, 162, COLOR_BACKGROUND);
  _tft->fillRect(507, 50, 293, 430, COLOR_BACKGROUND);
  // // add lines
  for(int i=0; i<sizeof(dial_lines)/sizeof(int)/4; i++) {
    _tft->drawLine(dial_lines[i*4], dial_lines[i*4+1], dial_lines[i*4+2], dial_lines[i*4+3], COLOR_DIAL);
  }
  // add font
  _tft->drawChar(380, 210, 'N', COLOR_FONT_SPEED, COLOR_BACKGROUND, 8);
  _tft->drawChar(430, 210, 'A', COLOR_FONT_SPEED, COLOR_BACKGROUND, 8);
  _tft->drawBitmap(477, 245, kph_bmp, 37, 19, COLOR_FONT_SPEED);
}

void Lcd::speed(int val) {
  if( val == _prev_speed ) return;
  if(val >= 0 && val <= 200) {
    // draw speed
    if( val > _prev_speed ) draw_speed( _prev_speed, val, true );
    else if ( val < _prev_speed ) draw_speed( val, _prev_speed, false );
    _prev_speed = val;

    // add font
    char a = val / 10 + '0';
    char b = val % 10 + '0';
    _tft->drawChar(380, 210, a, COLOR_FONT_SPEED, COLOR_BACKGROUND, 8);
    _tft->drawChar(430, 210, b, COLOR_FONT_SPEED, COLOR_BACKGROUND, 8);
  } else {
    draw_speed( 0, 60, false );
    _tft->drawChar(380, 210, 'N', COLOR_FONT_SPEED, COLOR_BACKGROUND, 8);
    _tft->drawChar(430, 210, 'A', COLOR_FONT_SPEED, COLOR_BACKGROUND, 8);
  }
}

void Lcd::fc_voltage(int val) {
  if( val == _prev_fc_voltage ) return;
  if(val >= 0 && val <= 46) {
    // draw fc voltage
    _prev_fc_voltage = val;
    char a = val / 10 + '0';
    char b = val % 10 + '0';
    //_tft->drawChar(380, 210, a, COLOR_FONT_SPEED, COLOR_BACKGROUND, 8);
    //_tft->drawChar(430, 210, b, COLOR_FONT_SPEED, COLOR_BACKGROUND, 8);
  } else {
    //_tft->drawChar(380, 210, 'N', COLOR_FONT_SPEED, COLOR_BACKGROUND, 8);
    //_tft->drawChar(430, 210, 'A', COLOR_FONT_SPEED, COLOR_BACKGROUND, 8);
  }
}

void Lcd::indictor(bool left, bool right) {
  if(left != _prev_left) {
    if(left) {
      _tft->fillTriangle(30, 400, 75, 370, 75, 430, COLOR_INDICTORS);
      _tft->fillRect(75, 382, 30, 35, COLOR_INDICTORS);
    } else {
      _tft->fillRect(27, 362, 90, 80, COLOR_BACKGROUND);
    }
    _prev_left = left;
  }
  if(right != _prev_right) {
    if(right) {
      _tft->fillTriangle(770, 400, 725, 370, 725, 430, COLOR_INDICTORS);
      _tft->fillRect(695, 382, 30, 35, COLOR_INDICTORS);
    } else {
      _tft->fillRect(695, 362, 80, 80, COLOR_BACKGROUND);
    }
    _prev_right = right;
  }
}

void Lcd::light(bool on) {
  if(on) {
    _tft->fillCircle(260, 400, 50, COLOR_BUTTON_LIGHT);
    _tft->drawBitmap(233, 375, light_bmp, 60, 49, COLOR_BUTTON_IMG);
  } else {
    _tft->fillCircle(260, 400, 50, COLOR_BUTTON_DARK);
    _tft->drawBitmap(233, 375, light_bmp, 60, 49, COLOR_BUTTON_IMG);
  }
}

void Lcd::wiper(bool on) {
  if(on) {
    _tft->fillCircle(540, 400, 50, COLOR_BUTTON_LIGHT);
    _tft->drawBitmap(510, 385, wipers_bmp, 60, 31, COLOR_BUTTON_IMG);
  } else {
    _tft->fillCircle(540, 400, 50, COLOR_BUTTON_DARK);
    _tft->drawBitmap(510, 385, wipers_bmp, 60, 31, COLOR_BUTTON_IMG);
  }
}

void Lcd::hazards(bool on) {
  if(on) {
    _tft->fillCircle(400, 400, 50, COLOR_BUTTON_LIGHT);
    _tft->drawBitmap(371, 370, hazards_bmp, 60, 53, COLOR_BUTTON_IMG);
  } else {
    _tft->fillCircle(400, 400, 50, COLOR_BUTTON_DARK);
    _tft->drawBitmap(371, 370, hazards_bmp, 60, 53, COLOR_BUTTON_IMG);
  }
}

void Lcd::highlight_button(int button) {
  if(_prev_hightlight != button) {
    // remove previous highlight
    switch(_prev_hightlight) {
    case LIGHT:
      _tft->drawCircle(260, 400, 56, COLOR_BACKGROUND);
      _tft->drawCircle(260, 400, 55, COLOR_BACKGROUND);
      _tft->drawCircle(260, 400, 54, COLOR_BACKGROUND);
      break;
    case WIPER:
      _tft->drawCircle(540, 400, 56, COLOR_BACKGROUND);
      _tft->drawCircle(540, 400, 55, COLOR_BACKGROUND);
      _tft->drawCircle(540, 400, 54, COLOR_BACKGROUND);
      break;
    case HAZARDS:
      _tft->drawCircle(400, 400, 56, COLOR_BACKGROUND);
      _tft->drawCircle(400, 400, 55, COLOR_BACKGROUND);
      _tft->drawCircle(400, 400, 54, COLOR_BACKGROUND);
      break;
    }

    // add current highlight
    switch(button) {
    case LIGHT:
      _tft->drawCircle(260, 400, 56, COLOR_BUTTON_DARK);
      _tft->drawCircle(260, 400, 55, COLOR_BUTTON_DARK);
      _tft->drawCircle(260, 400, 54, COLOR_BUTTON_DARK);
      break;
    case WIPER:
      _tft->drawCircle(540, 400, 56, COLOR_BUTTON_DARK);
      _tft->drawCircle(540, 400, 55, COLOR_BUTTON_DARK);
      _tft->drawCircle(540, 400, 54, COLOR_BUTTON_DARK);
      break;
    case HAZARDS:
      _tft->drawCircle(400, 400, 56, COLOR_BUTTON_DARK);
      _tft->drawCircle(400, 400, 55, COLOR_BUTTON_DARK);
      _tft->drawCircle(400, 400, 54, COLOR_BUTTON_DARK);
      break;
    }

    _prev_hightlight = button;
  }
}

void Lcd::draw_speed(int start, int stop, bool on) {
  for(int i=start; i<stop; i++) {
    _tft->drawLine(speed_lines[i*4], speed_lines[i*4+1], speed_lines[i*4+2], speed_lines[i*4+3], on ? COLOR_SPEED : COLOR_BACKGROUND);
  }
}
