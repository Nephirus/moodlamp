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
    uint8_t r = 255;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t state = 1;
    init();
    while (1){
        OCR1A = g;
        OCR1B = b;
        OCR2 = r;
        _delay_ms(30);
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
    return 0;
}
