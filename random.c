#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>


/*
 * CKSEL[3:0] = 0010 (default)
 * SUT = 10 (default)
 * CKDIV8 = 0 (default)
 * 
 * => f = 1 MHz
 * 
 */

void init(void);



int main(void){
	init();
	
	while(1)
		;
	
	return 0;
}

void init(void){
	// PB0, PB1, PB4 are outputs
	DDRB = DDB0 | DDB1 | DDB4;
	
	// PB2, PB3 pin change interrupt enable
	PCMSK = PCINT2 | PCINT3;
	GIMSK = PCIE;
	
	// Timer0 
	TCCR0A = COM0A1 | COM0B1 | WGM1 | WGM0;		// fast pwm, clear on compare match
	TCCR0B = CS00;	// no prescaler
	OCR0A = 0;	//duty cycle = 0
	OCR0B = 0;
	
	// Timer 1
	GTCCR = PWM1B | COM1B1;
	TCCR1 = CS10;
	OCR1C = 0xff;
	OCR1B = 0;
	
	// debounce timer interrupt
	TIMSK = OCIE1A;
}