/***********************************************************************
 * 
 * Knight Rider leds by Frantisek Langr
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_1   PB1     // AVR pin where LED is connected
#define LED_2   PB2
#define LED_3   PB3
#define LED_4   PB4
#define LED_5   PB5

#define BLINK_DELAY 250
#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions

/* Functions ---------------------------------------------------------*/
/**
 * Main function where the program execution begins. Knight Rider 
 */
int main(void)
{
    /* Pin definitions */
    // Set pin as output in Data Direction Register...
    DDRB = DDRB | (1<<LED_1);
	DDRB = DDRB | (1<<LED_2);
	DDRB = DDRB | (1<<LED_3);
	DDRB = DDRB | (1<<LED_4);
	DDRB = DDRB | (1<<LED_5);
    // ...and turn LED off in Data Register
    PORTB = PORTB & ~(1<<LED_1);
	PORTB = PORTB & ~(1<<LED_2);
	PORTB = PORTB & ~(1<<LED_3);
	PORTB = PORTB & ~(1<<LED_4);
	PORTB = PORTB & ~(1<<LED_5);


	
    // Infinite loop
    while (1)
    {
        // Blinking right
        _delay_ms(BLINK_DELAY);
		PORTB = PORTB ^ (1<<LED_1);
		_delay_ms(BLINK_DELAY);
		PORTB = PORTB ^ (1<<LED_1);	
		
	    _delay_ms(BLINK_DELAY);
		PORTB = PORTB ^ (1<<LED_2);
		_delay_ms(BLINK_DELAY);
		PORTB = PORTB ^ (1<<LED_2);
		
        _delay_ms(BLINK_DELAY);
        PORTB = PORTB ^ (1<<LED_3);
        _delay_ms(BLINK_DELAY);
        PORTB = PORTB ^ (1<<LED_3);
        
        _delay_ms(BLINK_DELAY);
        PORTB = PORTB ^ (1<<LED_4);
        _delay_ms(BLINK_DELAY);
        PORTB = PORTB ^ (1<<LED_4);
        
        _delay_ms(BLINK_DELAY);
        PORTB = PORTB ^ (1<<LED_5);
        _delay_ms(BLINK_DELAY);
        PORTB = PORTB ^ (1<<LED_5);
		
		// Blinking left
		_delay_ms(BLINK_DELAY);
		PORTB = PORTB ^ (1<<LED_4);
		_delay_ms(BLINK_DELAY);
		PORTB = PORTB ^ (1<<LED_4);
		
		_delay_ms(BLINK_DELAY);
		PORTB = PORTB ^ (1<<LED_3);
		_delay_ms(BLINK_DELAY);
		PORTB = PORTB ^ (1<<LED_3);
		
		_delay_ms(BLINK_DELAY);
		PORTB = PORTB ^ (1<<LED_2);
		_delay_ms(BLINK_DELAY);
		PORTB = PORTB ^ (1<<LED_2);
		
    }

    // Will never reach this
    return 0;
}