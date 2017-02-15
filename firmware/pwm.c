#include <avr/io.h>
#include <avr/interrupt.h>

extern unsigned char sintab[256];
unsigned char scaled_sintab[256];

static unsigned char pwm_state;
static unsigned char pwm_position;

unsigned char pwm_deadtime = 10;
unsigned char pwm_step = 1;
unsigned char pwm_amplitude = 80;
unsigned char pwm_prescale = 8;
char pwm_direction = 0;

static void compute_scaled_sintab(unsigned char mult);

void pwm_init()
{
	DDRD |= (1<<3) | (1<<5) | (1<<6);
	DDRB |= (1<<1) | (1<<2) | (1<<3);
	
	PORTD &= ~((1<<3) | (1<<5) | (1<<6));
	PORTB &= ~((1<<1) | (1<<2) | (1<<3));
}

static void compute_scaled_sintab(unsigned char mult)
{
	int i;
	unsigned int val;

	if (pwm_direction == 0) {
		for (i=0;i<256;i++) {
			val = mult * sintab[i];
			scaled_sintab[i] = val >> 8;
		}
	} else {
		for (i=0;i<256;i++) {
			val = mult * sintab[255-i];
			scaled_sintab[i] = val >> 8;
		}
	}
}

void pwm_enable()
{
	compute_scaled_sintab(pwm_amplitude);

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
	PORTD &= ~((1<<3) | (1<<5) | (1<<6));
	PORTB &= ~((1<<1) | (1<<2) | (1<<3));

	TCCR0A = 0;
	TCCR1A = 0;
	TCCR2A = 0;

	TCCR0B = 0;
	TCCR1B = 0;
	TCCR1B = 0;	

	TIMSK0 &= ~1;
}

void pwm_update_phases()
{
	unsigned char phase[3];
	switch (pwm_state) {
	case 0:
		phase[0] = scaled_sintab[pwm_position];
		phase[1] = 0;
		phase[2] = scaled_sintab[255-pwm_position];
		break;
	case 1:
		phase[1] = scaled_sintab[pwm_position];
		phase[2] = 0;
		phase[0] = scaled_sintab[255-pwm_position];
		break;
	case 2:
		phase[2] = scaled_sintab[pwm_position];
		phase[0] = 0;
		phase[1] = scaled_sintab[255-pwm_position];
		break;
	default:
		break;
	}
	OCR0A = phase[0] + pwm_deadtime;
	OCR0B = phase[0];

	OCR1A = phase[1] + pwm_deadtime;;
	OCR1B = phase[1];

	OCR2A = phase[2] + pwm_deadtime;;
	OCR2B = phase[2];
}

static unsigned char counter = 0;

ISR(TIMER0_OVF_vect)
{
	int commute = 0;

	counter++;

	if (counter >= pwm_prescale) {
		int index;

		pwm_position += pwm_step;
		if (pwm_position < pwm_step)
			commute = 1;

		if (commute) {
			if (pwm_direction == 0)
				pwm_state = (pwm_state + 1) % 3;
			else
				pwm_state = (pwm_state + 2) % 3;
			PORTC ^= (1<<5);
			TCNT0 = 0;
			TCNT1 = 0;
			TCNT2 = 0;

			commute = 0;
		}
		counter = 0;
	}
}
