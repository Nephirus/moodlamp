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

int main(){
    uint8_t i = 0;
    init();
    while (1){
        //OCR1A = i++;
        OCR1B = i++;
        //OCR2 = i++;
        _delay_ms(10);
    }
    return 0;
}
