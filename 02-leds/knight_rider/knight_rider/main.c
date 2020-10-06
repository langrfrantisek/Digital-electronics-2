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

//Variable for cycles
uint8_t i;

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
	
	// turn LED on in Data Register
	PORTB = PORTB | (1<<LED_1);
    // turn LED off in Data Register
	PORTB = PORTB & ~(1<<LED_2);
	PORTB = PORTB & ~(1<<LED_3);
	PORTB = PORTB & ~(1<<LED_4);
	PORTB = PORTB & ~(1<<LED_5);
	
    // Infinite loop
    while (1)
    {
		// Blinking right
		for (i = 0; i < 4; i++)
		{
			_delay_ms(BLINK_DELAY);
			PORTB <<=1;
		}
		
		// Blinking left
		for (i = 4; i > 0; i--)
		{
			_delay_ms(BLINK_DELAY);
			PORTB >>=1;
		}

    }

    // Will never reach this
    return 0;
}