Door Control Firmware
=====================

This project combines PWM motor control, RFC reader and bolt control.
The microcontroller used is an ATMega328P. The pinout is:

| DIP |Ardu| Pin  |
| Pin |Pin#| Name | Function | Notes                               |
| ----|----| ---- | -------- | -----------------                   |
|   1 |    | PC6  | RESET    |                                     |
|   2 |  0 | PD0  | RXD      |                                     |
|   3 |  1 | PD1  | TXD      |                                     |
|   4 |  2 | PD2  |          |                                     |
|   5 |  3 | PD3  | OC2B     | Phase 2                             |
|   6 |  4 | PD4  |          |                                     |
|   7 |    | VCC  | VCC      | 3.3V                                |
|   8 |    | GND  | GND      |                                     |
|   9 |XTAL| PB6  |          |                                     |
|  10 |XTAL| PB7  |          |                                     |
|  11 |  5 | PD5  | OC0B     | Phase 0                             |
|  12 |  6 | PD6  | OC0A     | Phase 0 complement                  |
|  13 |  7 | PD7  |          |                                     |
|  14 |  8 | PB0  |          |                                     |
|  15 |  9 | PB1  | OC1A     | Phase 1 complement                  |
|  16 | 10 | PB2  | OC1B     | Phase 1                             |
|  17 | 11 | PB3  | OC2A     | Phase 0 complement                  |
|  18 | 12 | PB4  | MISO     | SPI bus for MRC522                  |
|  19 | 13 | PB5  | SCK      | (ditto)                             |
|  20 |    | AVCC |          |                                     |
|  21 |    | AREF |          |                                     |
|  22 |    | GND  |          |                                     |
|  23 | A0 | PC0  | ADC0     | Hall sensor 0                       |
|  24 | A1 | PC1  | ADC1     | Hall sensor 1                       |
|  25 | A2 | PC2  | MOSI     | Bit-banged MOSI                     |
|  26 | A3 | PC3  | SS#      | Slave select for MFRC522            |
|  27 | A4 | PC4  |          |                                     |
|  28 | A5 | PC5  | PC5      | Currently an active high LED (10mA) |



