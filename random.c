#define F_CPU 1000000

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <math.h>


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
volatile uint8_t r = 0;
volatile uint8_t g = 80;
volatile uint8_t b = 160;
volatile uint8_t dr, dg, db;


#define THRESHOLD 10

void getRGB(uint8_t hue){
	float h,x;
	h = hue/42.5;
	x = 255*(1-fabs(h%2 - 1));
	if(h>=0 && h<1){
		r = 255;
		g = x;
		b = 0;
	} else if(h>=1 && h<2){
		r = x;
		g = 255;
		b = 0;
	} else if(h>=2 && h<3){
		r = 0;
		g = 255;
		b = x;
	} else if(h>=3 && h<4){
		r = 0;
		g = x;
		b = 255;
	} else if(h>=4 && h<5){
		r = x;
		g = 0;
		b = 255;
	} else if(h>=5 && h<6){
		r = 255;
		g = 0;
		b = x;
	}
	
}

ISR (TIM1_OVF_vect)           /* Note [2] */
{
    //static uint8_t pwm;        /* Note [3] */
    //static uint8_t direction;
	static uint8_t cnt=0;
	if(++cnt==THRESHOLD){
    switch (dg)          /* Note [4] */
    {
        case UP:
            if (++g == 255)
                dg = DOWN;
            break;

        case DOWN:
            if (--g == 0)
                dg = UP;
            break;
    }
    switch (db)          /* Note [4] */
    {
        case UP:
            if (++b == 255)
                db = DOWN;
            break;

        case DOWN:
            if (--b == 0)
                db = UP;
            break;
    }
	
		OCR1A = g;                  /* Note [5] */
		OCR1B = b;
		cnt = 0;
	}
}

ISR (TIM0_OVF_vect)           /* Note [2] */
{
    //static uint8_t pwm=160;        /* Note [3] */
    //static uint8_t direction;
	static uint8_t cnt=0;
	if(++cnt==THRESHOLD){
    switch (dr)          /* Note [4] */
    {
        case UP:
            if (++r == 255)
                dr = DOWN;
            break;

        case DOWN:
            if (--r == 0)
                dr = UP;
            break;
    }
    
	
		OCR0A = r;
		cnt = 0;
	}
}

int main(void){
	init();

	while(1){
		sleep_mode();
	}
	return 0;
}

void init(void){
	/* green - OCR1A - PB1 */
	TCCR1 = _BV(PWM1A) | _BV(COM1A1);
    TCCR1 |= _BV(CS12);
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

	
    TIMSK = _BV (TOIE1);
    sei ();
}
