/* 
 * Serial command interpreter
 */
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "config.h"
#include "serial.h"

#define is_valid_char(_ch) (			\
	('a' <= _ch && _ch <= 'z') ||		\
	('A' <= _ch && _ch <= 'Z') ||		\
	('0' <= _ch && _ch <= '9') ||		\
	(_ch == ' '))

#define is_crlf(_ch)     ((_ch == '\n') || (_ch == '\r'))

/* states */
enum {
	INVALID = 0,
	START_OF_LINE,
	IN_COMMAND,
};

static int state = START_OF_LINE;
static int index = 0;
static char buf[SERIAL_BUFSIZE];

/* Public variables */
int serial_argc;
char *serial_argv[MAX_ARGS];

/* Public functions */
void serial_init();
void serial_putc(unsigned char c);
void serial_write(const char *buf, int len);
void serial_rx_disable();
void serial_rx_enable();

static void parse_args()
{
	char *p = buf;
	serial_argc = 0;

	while (*p) {
		serial_argv[serial_argc++] = p;
		while (*p != ' ' && *p)
			p++;		
		while (*p == ' ')
			*p++ = 0;
		if (serial_argc == MAX_ARGS)
			break;
	}
}

/* 
 * Serial command state machine (driven by ISR):
 *
 *                                    [*]
 *        ________________________________________________________
 *       /                                                        \
 *      |          [*]                   [CRLF] (execute)          |
 *      |    __________________      __________________________    |
 *      |   /                  \    /                          \   |
 *      |  |                    |  |                            |  |
 *      v  v     [CRLF]         |  v      [START_COMMAND]       |  v
 *    INVALID  ---------> START_OF_LINE ------------------> IN_COMMAND
 *      |  ^                    |  ^                            |  ^
 *      |  |                    |  |                            |  |
 *       \/                      \/                              \/
 *       [*]                   [CRLF]                       [valid_char]
 *                                                            (store)
 */
static void process_char(char c)
{
	switch (state) {
	case INVALID:
		serial_argc = 0;
		if (is_crlf(c))
			state = START_OF_LINE;
		break;
	case START_OF_LINE:
		index = 0;
		serial_argc = 0;
		if (is_crlf(c)) 
			break;
		if (c == SERIAL_START_COMMAND)
			state = IN_COMMAND;
		else
			state = INVALID;
		break;
	case IN_COMMAND:
		if (is_crlf(c)) {
			buf[index] = 0;
			parse_args();
			state = START_OF_LINE;
			serial_rx_disable();
			break;
		}

		if (is_valid_char(c) && (index < SERIAL_BUFSIZE-1)) {
			buf[index++] = c;
			break;
		}

		state = INVALID;
	default:
		break;
	}
}

/* 
 * Serial RX ISR
 */
ISR(USART_RX_vect)
{
	if (UCSR0A & (1<<RXC0))
		process_char(UDR0);
}


/* Exported functions */
void serial_putc(unsigned char c)
{
	while ( !( UCSR0A & (1<<UDRE0)) );  // wait for transmit buffer to be ready.
	UDR0 = c;
}


void serial_write(const char *buf, int len)
{
	while (len--)
		serial_putc(*buf++);
}

void serial_rx_enable()
{
	UCSR0B |= (1<<RXCIE0);
}

void serial_rx_disable()
{
	UCSR0B &= ~(1<<RXCIE0);
}

void serial_init()
{
	const unsigned int divider = (F_CPU  / 16 / SERIAL_BAUDRATE) - 1;
	
	UBRR0H = (unsigned char)(divider >> 8);
	UBRR0L = (unsigned char) divider;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);   /* Enable read/write */
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); /* 8bit */
}
