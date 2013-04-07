/*
 *  Moodlamp v2 firmware
 *  Author: Nephirus <nephirus@nephirus.cz>
 *
 *  Moodlamp is a programmable RGB LED lamp driven by ATmega8A MCU.
 */

/*
 * Red      PB3 (OC2)
 * Green    PB2 (OC1A)
 * Blue     PB1 (OC1B)
 */

#define F_CPU 1000000UL

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void init();
void clear_all();
void test_colors();
void rainbow_update();

volatile uint8_t r = 255;
volatile uint8_t g = 0;
volatile uint8_t b = 0;
volatile uint8_t state = 1;
volatile uint8_t rainbow_step = 20;    // rainbow step size in ms



void init(){
    // Set direction of LED pins to output
    DDRB |= (1<<DDB3) | (1<<DDB2) | (1<<DDB1);

    // Setup buttons
    DDRD |= (1<<PD6);   // S2 -> input
    PORTD |= (1<<PD6);  // enable internal pullup

    DDRD |= (1<<PD3);   // S1 -> input
    PORTD |= (1<<PD3);  // enable internal pullup


    // Configure Timer0 
    TIMSK |= (1<<TOIE0);
    TCCR0 |= (1<<CS02) | (1<<CS00);     // clk/1024
    
    // Configure Timer2 (red PWM)
    TCCR2 |= (1<<WGM21) | (1<<WGM20);   // Fast PWM mode
    TCCR2 |= (1<<COM21);   // Clear OC2 on Compare Match, set at bottom
    TCCR2 |= (1<<CS20);     // no prescaling

    // Configure Timer1 (green and blue PWM)
    TCCR1A |= (1<<WGM12) | (1<<WGM10);  // Fast PWM, 8-bit
    TCCR1A |= (1<<COM1A1) | (1<<COM1B1);    // Clear OC1{A,B} on Compare Match, set at bottom
    TCCR1B |= (1<<CS10);    // no prescaling
}

void clear_all(){
    PORTB &= ~((1<<PB3) | (1<<PB2) | (1<<PB1));
}

void test_colors(){
    PORTB |= (1<<PB3) | (1<<PB2) | (1<<PB1);
    _delay_ms(2500);
    clear_all();
    PORTB |= (1<<PB3);
    _delay_ms(2500);
    clear_all();
    PORTB |= (1<<PB2);
    _delay_ms(2500);
    clear_all();
    PORTB |= (1<<PB1);
    _delay_ms(2500);
    clear_all();
}

void rainbow_update(){
    OCR1A = g;
    OCR1B = b;
    OCR2 = r;
    switch (state){
        case 1:
            g++;
            if(g == 255)
                state++;
            break;
        case 2:
            r--;
            if(r == 0)
                state++;
            break;
        case 3:
            b++;
            if(b == 255)
                state++;
            break;
        case 4:
            g--;
            if(g == 0)
                state++;
            break;
        case 5:
            r++;
            if(r == 255)
                state++;
            break;
        case 6:
            b--;
            if(b == 0)
                state = 1;
            break;
    }
}

volatile uint8_t s1state = 0;
volatile uint8_t s1trig = 0;
volatile uint8_t s2state = 0;
volatile uint8_t s2trig = 0;

ISR(TIMER0_OVF_vect, ISR_NOBLOCK){
    rainbow_update();
    TCNT0 = 255 - rainbow_step;

    // input check - FIXME: checking interval should not depend on rainbow speed
    if(!(PIND & _BV(PD6))){
        s2state = 1;
    } else {
        s2state = 0;
        s2trig = 0;
    }

    if(!(PIND & _BV(PD3))){
        s1state = 1;
    } else {
        s1state = 0;
        s1trig = 0;
    }
}

int main(){
    init();
    sei();
    while (1){
        if(s2state && !s2trig){
            rainbow_step += 5;
            s2trig = 1;
        }
        if(s1state && !s1trig){
            rainbow_step -= 5;
            s1trig = 1;
        }
    }
    return 0;
}


