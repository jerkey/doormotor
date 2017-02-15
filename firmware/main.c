#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "serial.h"

static int led_command()
{
	if (strcmp(serial_argv[1], "off") == 0)
		PORTC &= ~(1<<5);
	else if (strcmp(serial_argv[1], "on") == 0)
		PORTC |= (1<<5);
	else
		return -1;

	return 0;
}

extern unsigned char pwm_deadtime, pwm_step, pwm_prescale, pwm_amplitude, pwm_direction;

static int pwm_command()
{
	char *arg = serial_argv[1];

	if (serial_argc < 2)
		return -1;

	if (strcmp(serial_argv[1], "off") == 0) {
		pwm_disable();
	} else if (strcmp(serial_argv[1], "on") == 0) {
		pwm_enable();
	} else if (strcmp(serial_argv[1], "amp") == 0) {
		if (serial_argc != 3)
			return -1;
		pwm_amplitude = atoi(serial_argv[2]);
	} else if (strcmp(serial_argv[1], "dead") == 0) {
		if (serial_argc != 3)
			return -1;
		pwm_deadtime = atoi(serial_argv[2]);
	} else if (strcmp(serial_argv[1], "step") == 0) {
		if (serial_argc != 3)
			return -1;
		pwm_step = atoi(serial_argv[2]);
	} else if (strcmp(serial_argv[1], "pre") == 0) {
		if (serial_argc != 3)
			return -1;
		pwm_prescale = atoi(serial_argv[2]);
	} else if (strcmp(serial_argv[1], "dir") == 0) {
		if (serial_argc != 3)
			return -1;
		pwm_direction = atoi(serial_argv[2]);
	} else {
		return -1;
	}

	return 0;
}

static int mfrc_command()
{
	if (serial_argc == 2) {	/* read */
		unsigned char addr = atoi(serial_argv[1]);
		unsigned char val;
		char outstr[20];

		val = mfrc522_get(addr);
		sprintf(outstr, "=0x%x\r\n", val);
		serial_write(outstr, strlen(outstr));
		return 1;
	} else if (serial_argc == 3) {
		unsigned char addr = atoi(serial_argv[1]);
		unsigned char val = atoi(serial_argv[2]);
		
		mfrc522_set(addr, val);
		return 0;
	}

	return -1;
}


static unsigned char adc_source = 0;
static unsigned int adc_value[2];

static int adc_command()
{
	char outstr[20];
	int len;

	len = sprintf(outstr, "=0x%x 0x%x\r\n", adc_value[0], adc_value[1]);
	serial_write(outstr, len);
	return 1;
}

static void adc_start_conversion()
{
	ADMUX = (1<<REFS0) | adc_source;

	ADCSRA &= ~(1<<4);	   /* clear interrupt flag */
	ADCSRA |= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADEN);
	ADCSRA |= (1<<ADSC);	/* start conversion */
}

static void adc_update()
{
	if (ADCSRA & (1<<6))
		return;

	adc_source ^= 1;
	adc_value[adc_source] = ADC;

	adc_start_conversion();
}

#define CMD(_cmd)     (strcmp(serial_argv[0], _cmd) == 0)
void main()
{
	DDRC |= (1<<5);
	
	serial_init();
	mfrc522_init();
	pwm_init();
	sei();

	serial_rx_enable();

	ADCSRA = (1<<7);	/* ADEN */
	ADCSRB = 0;

	adc_start_conversion();

	while (1) {
		if (serial_argc) {
			int result;

			if (CMD("led"))
				result = led_command();
			else if (CMD("mfrc"))
				result = mfrc_command();
			else if (CMD("pwm"))
				result = pwm_command();
			else if (CMD("adc"))
				result = adc_command();
			else
				result = 1;

			if (result<0)
				serial_write("ERROR\r\n", 7);
			else if (result==0)
				serial_write("OK\r\n", 4);
			/* all other return codes return a message */

			serial_argc = 0;
			serial_rx_enable();
		}
		pwm_update_phases();
		adc_update();
	}
}
