#include <avr/io.h>
#include <avr/interrupt.h>

extern unsigned char sintab[256];
unsigned char scaled_sintab[256];

static unsigned char pwm_state;
static unsigned char pwm_position;

unsigned char pwm_deadtime = 10;
unsigned char pwm_step = 1;
unsigned char pwm_amplitude = 128;
unsigned char pwm_prescale = 0;
char pwm_direction = 1;

static void compute_scaled_sintab(unsigned char mult);

void pwm_init()
{
	DDRD |= (1<<3) | (1<<5) | (1<<6);
	DDRB |= (1<<1) | (1<<2) | (1<<3);
	
	
	/* Timer 0 */
	TCCR0A =  (3<<6)	/* SetOC0A on compare match, clear at BOTTOM */
		| (2<<4)	/* Clear OC0B on compare match, set at BOTTOM */
		| (1<<0);	/* Phase-correct PWM 0x00 to 0xff */

	/* Timer 1 */
	TCCR1A =  (3<<6)
		| (2<<4)
		| (1<<0);

	/* Timer 2 */
	TCCR2A =  (3<<6)
		| (2<<4)
		| (1<<0);

	pwm_state = 0;
}

static void compute_scaled_sintab(unsigned char mult)
{
	int i;
	unsigned int val;

	for (i=0;i<256;i++) {
		val = mult * sintab[i];
		scaled_sintab[i] = val >> 8;
	}
}

void pwm_enable()
{
	compute_scaled_sintab(pwm_amplitude);

	TCNT0 = 0;
	TCNT1H = 0;
	TCNT1L = 0;
	TCNT2 = 0;

	pwm_state = 0;
	pwm_position = 0;
	
	TCCR0B = (1<<0);	/* No prescaling, so 8MHz/256 = 31.25kHz */
	TCCR1B = (1<<0);
	TCCR2B = (1<<0);

	TIMSK0 |= 1;
}

void pwm_disable()
{
	TCCR0B = 0;
	TCCR1B = 0;
	TCCR1B = 0;	

	TIMSK0 &= ~1;
}

void pwm_update_phases()
{
	unsigned char phase[3];
	
	phase[pwm_state] = scaled_sintab[pwm_position];
	phase[(pwm_state+pwm_direction) % 3] = 0;
	phase[(pwm_state-pwm_direction) % 3] = scaled_sintab[255-pwm_position];

	OCR0A = phase[0];
	OCR0B = phase[0] + pwm_deadtime;

	OCR1A = phase[1];
	OCR1B = phase[1] + pwm_deadtime;

	OCR2A = phase[2];
	OCR2B = phase[2] + pwm_deadtime;
}

static unsigned char counter = 0;

ISR(TIMER0_OVF_vect)
{
	counter++;

	if (counter >= pwm_prescale) {
		int index;

		pwm_position += pwm_step;

		/* commutation on overflow */
		if (pwm_position < pwm_step) {
			pwm_state = (pwm_state + 1) % 3;
			PORTC ^= (1<<5);
		}
		counter = 0;
	}
}
