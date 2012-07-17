#define F_CPU 1000000

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>


/*
 * CKSEL[3:0] = 0010 (default)
 * SUT = 10 (default)
 * CKDIV8 = 0 (default)
 * 
 * => f = 1 MHz
 * 
 */

void init(void);


enum { UP, DOWN };
volatile uint8_t r = 255;
volatile uint8_t g = 0;
volatile uint8_t b = 0;
volatile uint8_t dr, dg, db;

volatile uint8_t phase = 0;
volatile uint8_t cnt=0;

volatile uint8_t threshold;

volatile uint8_t input1 = 1;
volatile uint8_t input2 = 1;
volatile uint8_t input1_cnt=0;
volatile uint8_t input2_cnt=0;

const uint8_t thr[6] = {1,2,5,10,50,200};


ISR (TIM1_OVF_vect){
	
	if(++cnt>=threshold){
		switch (phase){
			case 0:
				if(++g == 255){
					phase++;
				}
				break;
			case 1:
				if(--r == 0){
					phase++;
				}
				break;
			case 2:
				if(++b == 255){
					phase++;
				}
				break;
			case 3:
				if(--g == 0){
					phase++;
				}
				break;
			case 4:
				if(++r == 255){
					phase++;
				}
				break;
			case 5:
				if(--b == 0){
					phase = 0;
				}
				break;
		}
		
		OCR1A = g;                
		OCR1B = b;
		OCR0A = r;
		cnt = 0;
	}
	
}


int main(void){
	init();
	uint8_t i=4;
	threshold = thr[i];

	while(1){
		if(((PINB & _BV(PB2))) == 0){
			if(i>0){
				threshold = thr[--i];
			}
			_delay_ms(500);
		}
		if(((PINB & _BV(PB3))) == 0){
			if(i<5){
				threshold = thr[++i];
			}
			_delay_ms(500);
		}
	}
	return 0;
}

void init(void){
	/* green - OCR1A - PB1 */
	TCCR1 = _BV(PWM1A) | _BV(COM1A1);
    TCCR1 |= _BV(CS11);
    OCR1A = 0;
    DDRB |= _BV (PB1);
	
	/* blue - OCR1B - PB4 */
	GTCCR = _BV(PWM1B) | _BV(COM1B1);
	OCR1B = 0;
	DDRB |= _BV (PB4);
	
	/* red - OCR0A - PB0 */
	TCCR0A = _BV(COM0A1) | _BV(WGM00) | _BV(WGM01);
	TCCR0B = _BV(CS01);
	OCR0A = 0;
	DDRB |= _BV(PB0);

	/* input buttons */
	PORTB |= _BV(PB2) | _BV(PB3);
	
    TIMSK = _BV (TOIE1);
    sei ();
}
