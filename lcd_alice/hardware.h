#ifndef HARDWARE_H
#define HARDWARE_H

// LCD Connections
// Connect SCLK to UNO Digital #13 (Hardware SPI clock)
// Connect MISO to UNO Digital #12 (Hardware SPI MISO)
// Connect MOSI to UNO Digital #11 (Hardware SPI MOSI)
int const RA8875_INT = 2;
int const RA8875_CS = 7;
int const RA8875_RESET = 3;

// CAN Bus Connections
const int CAN_INT = 9;

#endif
