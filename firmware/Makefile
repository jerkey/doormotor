CC=avr-gcc # make knows CC is the compiler it will use to create .o from .c
AVRDUDE=avrdude
AVRDUDEOPTS=-cusbtiny -pm328p
CFLAGS=-mmcu=atmega328p -DF_CPU=8000000UL # make knows to use CFLAGS with CC
OBJS=main.o serial.o pwm.o spi.o sintab.o

all: firmware.elf # all is the default when you run make, and firmware.elf is its depndant

sintab.c: # if you need sintab.o then first you need sintab.c.  to get it, do the below command:
	python sintab.py >sintab.c # this is how you get sintab.c if you don't have it already.

firmware.elf: $(OBJS) # to get firmware.elf first you need to have main.o serial.o pwm.o spi.o sintab.o
	$(CC) $(CFLAGS) $(OBJS) -o firmware.elf # then you should compile (link?) them to firmware.elf

flash: firmware.elf # to flash you need firmware.elf
	$(AVRDUDE) $(AVRDUDEOPTS) -U flash:w:firmware.elf # then you do this to flash

# E: ff, H:d9, L:e2
fuses: # to make fuses you don't need anything, you just run the below command
	$(AVRDUDE) $(AVRDUDEOPTS) -U efuse:w:0xff:m -U hfuse:w:0xd9:m -U lfuse:w:0xe2:m
clean: # to make clean you don't need anything, you just run the below command
	rm -fr *.o firmware.elf sintab.c

