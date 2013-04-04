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
#include <avr/sleep.h>


void init();
void clear_all();
void test_colors();

void init(){
    // Set direction of LED pins to output
    DDRB |= (1<<DDB3) | (1<<DDB2) | (1<<DDB1);
}

void clear_all(){
    PORTB &= ~((1<<PB3) | (1<<PB2) | (1<<PB1));
    //PORTB = 0;
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

void main(){
    init();
    while (1){
        test_colors();
    }
}
