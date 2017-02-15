CC=avr-gcc
AVRDUDE=avrdude
AVRDUDEOPTS=-cusbtiny -pm328p
AVRDUDEUPLOAD=-carduino -P/dev/ttyUSB0 -b115200 -D -pm328p
MHZ=16
CFLAGS=-mmcu=atmega328p -DF_CPU=$(MHZ)000000UL
OBJS=main.o serial.o pwm.o spi.o sintab.o

all: firmware.elf

sintab.c:
	python sintab.py >sintab.c

firmware.elf: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o firmware.elf

flash: firmware.elf
	$(AVRDUDE) $(AVRDUDEOPTS) -U flash:w:firmware.elf

upload: firmware.elf
	$(AVRDUDE) $(AVRDUDEUPLOAD) -U flash:w:firmware.elf

# E: ff, H:d9, L:e2
fuses:
	$(AVRDUDE) $(AVRDUDEOPTS) -U efuse:w:0xff:m -U hfuse:w:0xd9:m -U lfuse:w:0xe2:m
clean:
	rm -fr *.o firmware.elf sintab.c

