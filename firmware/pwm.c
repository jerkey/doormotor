#include <avr/io.h>
#include <avr/interrupt.h>

extern unsigned char sintab[256];
unsigned char scaled_sintab[256];

static unsigned char pwm_state;
static unsigned char pwm_position;

unsigned char pwm_deadtime = 10;
unsigned char pwm_step = 1;
unsigned char pwm_amplitude = 50;
unsigned char pwm_prescale = 50;
char pwm_direction = 1;

static void compute_scaled_sintab(unsigned char mult);

void pwm_init()
{
	DDRD |= (1<<3) | (1<<5) | (1<<6); // data direction registers     PD3, PD5, PD6
	DDRB |= (1<<1) | (1<<2) | (1<<3); // setting all these as outputs PB1, PB2, PB3
	
	PORTD &= ~((1<<3) | (1<<5) | (1<<6)); // PD3 = OC2B, PD5 = OC0B, PD6 = OC0A
	PORTB &= ~((1<<1) | (1<<2) | (1<<3)); // PB1 = OC1A, PB2 = OC1B, PB3 = OC2A
}

static void compute_scaled_sintab(unsigned char mult) // to get less than 100% of supply voltage
{
	int i;
	unsigned int val;

	for (i=0;i<256;i++) {
		val = mult * sintab[i];
		scaled_sintab[i] = val >> 8; // shr back down to 8 bit after multiplication
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
	TCCR0A = 0; // set PWM values to zero
	TCCR1A = 0;
	TCCR2A = 0;

	TCCR0B = 0;
	TCCR1B = 0;
	TCCR1B = 0;	

	TIMSK0 &= ~1;

	PORTD &= ~((1<<3) | (1<<5) | (1<<6)); // set outputs LOW
	PORTB &= ~((1<<1) | (1<<2) | (1<<3)); // set outputs LOW
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
	counter++;

	if (counter >= pwm_prescale) {
		int index;

		pwm_position += pwm_step;

		/* commutation on overflow */
		if (pwm_position < pwm_step) {
			pwm_state = (pwm_state + 1) % 3;
			PORTC ^= (1<<5);
			TCNT0 = 0;
			TCNT1 = 0;
			TCNT2 = 0;
		}
		counter = 0;
	}
}
