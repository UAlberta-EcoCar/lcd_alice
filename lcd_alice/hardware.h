#ifndef HARDWARE_H
#define HARDWARE_H

// LCD Connections
// Connect SCLK to UNO Digital #13 (Hardware SPI clock)
// Connect MISO to UNO Digital #12 (Hardware SPI MISO)
// Connect MOSI to UNO Digital #11 (Hardware SPI MOSI)
int const RA8875_INT = 2;
int const RA8875_CS = 7;
int const RA8875_RESET = 3;

// Joystick Connections
int const JOY_LEFT_1 = A3;
int const JOY_RIGHT_1 = A1;
int const JOY_DOWN_1 = A4;
int const JOY_UP_1 = A5;
int const JOY_PUSH_1 = A2;

int const JOY_LEFT_2 = 4;
int const JOY_RIGHT_2 = 6;
int const JOY_DOWN_2 = 5;
int const JOY_UP_2 = A0;
int const JOY_PUSH_2 = 8;

// CAN Bus Connections
const int CAN_INT = 9;

#endif
