/* 
 * SPI with GPIO slave select and bit-banged transmit
 *
 * SS#  = PC3
 * MOSI = PC2
 * MISO = PB4 (normal)
 * SCK  = PB5 (normal)
 *
 */
#include <avr/io.h>

#define nSS    (1<<3)
#define MOSI   (1<<2)
#define SCK    (1<<5)

#define SCK_LOW()           PORTB &= ~SCK
#define SCK_HIGH()          PORTB |= SCK

#define MOSI_LOW()          PORTC &= ~MOSI
#define MOSI_HIGH()         PORTC |= MOSI

#define SELECT_MRFC522()    PORTC &= ~nSS
#define DESELECT_MRFC522()  PORTC |= nSS

void mfrc522_init()
{
	DDRC |= (1<<2) | (1<<3);
	DDRB |= (1<<5);
	DDRB &= ~(1<<4);

	DESELECT_MRFC522();
	MOSI_LOW();
	SCK_LOW();
}

static void bitbang_byte(unsigned char x)
{
	int y=0xff;

	while (y) {
		SCK_LOW();
		if (x & 0x80)
			MOSI_HIGH();
		else
			MOSI_LOW();
		SCK_HIGH();
		y >>= 1;
		x <<= 1;
	}

	/* idle low */
	MOSI_LOW();
	SCK_LOW();
}

unsigned char mfrc522_get(unsigned char addr)
{
	unsigned char data;

	/* Turn off SPI peripheral */
	SCK_LOW();
	SPCR = 0;

	SELECT_MRFC522();
	bitbang_byte(addr);

	/* The next addr must be 0. */
	MOSI_LOW();
	
	/* Turn on the SPI peripheral */
	SPCR = (1<<6) | (1<<4);
	SPDR = 0;

	while (!(SPSR & 0x80));

	data = SPDR;
	DESELECT_MRFC522();

	SPCR = 0;
	return data;
}

void mfrc522_set(unsigned char addr, unsigned char val)
{
	SCK_LOW();
	SPCR = 0;

	SELECT_MRFC522();
	bitbang_byte(addr);
	bitbang_byte(val);
	MOSI_LOW();
	DESELECT_MRFC522();
}
