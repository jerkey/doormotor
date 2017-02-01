CC=avr-gcc
AVRDUDE=avrdude
AVRDUDEOPTS=-cusbtiny -pm328p
CFLAGS=-mmcu=atmega328p -DF_CPU=8000000UL
OBJS=main.o serial.o pwm.o spi.o sintab.o

all: firmware.elf

sintab.c:
	python sintab.py >sintab.c

firmware.elf: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o firmware.elf

flash: firmware.elf
	$(AVRDUDE) $(AVRDUDEOPTS) -U flash:w:firmware.elf

clean:
	rm -fr *.o firmware.elf sintab.c

