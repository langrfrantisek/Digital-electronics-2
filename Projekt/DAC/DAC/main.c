/*
 * DAC.c
 *
 * Created: 26.11.2020 21:39:16
 * Author : Fraja
 */ 

/* Defines -----------------------------------------------------------*/
#define pin0 PD0
#define pin1 PD1
#define pin2 PD2
#define pin3 PD3
#define pin4 PD4
#define pin5 PD5
#define pin6 PD6
#define pin7 PD7

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "gpio.h"           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC

int main(void)
{
    GPIO_config_output(&DDRD, pin0);
    GPIO_config_output(&DDRD, pin1);
    GPIO_config_output(&DDRD, pin2);
    GPIO_config_output(&DDRD, pin3);
    GPIO_config_output(&DDRD, pin4);
    GPIO_config_output(&DDRD, pin5);
    GPIO_config_output(&DDRD, pin6);
    GPIO_config_output(&DDRD, pin7);
    
    /* Configuration of 8-bit Timer/Counter0 */
    TIM0_overflow_1ms();
    TIM0_overflow_interrupt_enable();
    
    // Enables interrupts by setting the global interrupt mask
    sei();
    
    while (1) 
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */             
    }
    
    // Will never reach this
    return 0;
}

/* Interrupt service routine ----------------------------------------*/
ISR(TIMER0_OVF_vect)
{
	static int16_t number_of_overflows = 0;
    static uint8_t value = 0;
    
    PORTD = value;
        
    if (number_of_overflows < 256)
    {        
        value++;
    } 
    else if (number_of_overflows < 511)
    {
        value--;
    }
    else
    {
        number_of_overflows = 0;
    }    
    number_of_overflows++;
    
}