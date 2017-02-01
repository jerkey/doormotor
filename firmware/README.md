Door Control Firmware
=====================

This project combines PWM motor control, RFC reader and bolt control.
The microcontroller used is an ATMega328P. The pinout is:


| Pin  | Name | Function | Notes                               |
| ---- | ---- | -------- | ----------------- |
|   1 | PC6  |          |                                     |
|   2 | PD0  | RXD      |                                     |
|   3 | PD1  | TXD      |                                     |
|   4 | PD2  |          |                                     |
|   5 | PD3  | OC2B     | Phase 2                             |
|   6 | PD4  |          |                                     |
|   7 | VCC  | VCC      | 3.3V                                |
|   8 | GND  | GND      |                                     |
|   9 | PB6  |          |                                     |
|  10 | PB7  |          |                                     |
|  11 | PD5  | OC0B     | Phase 0                             |
|  12 | PD6  | OC0A     | Phase 0 complement                  |
|  13 | PD7  |          |                                     |
|  14 | PB0  |          |                                     |
|  15 | PB1  | OC1A     | Phase 1 complement                  |
|  16 | PB2  | OC1B     | Phase 1                             |
|  17 | PB3  | OC2A     | Phase 0 complement                  |
|  18 | PB4  | MISO     | SPI bus for MRC522                  |
|  19 | PB5  | SCK      | (ditto)                             |
|  20 | AVCC |          |                                     |
|  21 | AREF |          |                                     |
|  22 | GND  |          |                                     |
|  23 | PC0  | ADC0     | Hall sensor 0                       |
|  24 | PC1  | ADC1     | Hall sensor 1                       |
|  25 | PC2  | MOSI     | Bit-banged MOSI                     |
|  26 | PC3  | SS#      | Slave select for MFRC522            |
|  27 | PC4  |          |                                     |
|  28 | PC5  | PC5      | Currently an active high LED (10mA) |



