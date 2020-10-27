/***********************************************************************
 * 
 * Decimal counter with 7-segment output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "segment.h"        // Seven-segment display library for AVR-GCC

uint8_t singles = 0, decimals = 0;

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Display decimal 
 * counter values on SSD (Seven-segment display) when 16-bit 
 * Timer/Counter1 overflows.
 */
int main(void)
{
    // Configure SSD signals
    SEG_init();
	
	/* Configure 8-bit Timer/Counter0
     * Set prescaler and enable overflow interrupt */
	TIM0_overflow_4ms();
	TIM0_overflow_interrupt_enable();
	
    /* Configure 16-bit Timer/Counter1
     * Set prescaler and enable overflow interrupt */
	TIM1_overflow_262ms();
	TIM1_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
	sei();
	
    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/

/**
 * ISR starts when Timer/Counter0 overflows. Display values on SSD
 * value and display it on SSD.
 */
ISR(TIMER0_OVF_vect)
{
	static uint8_t pos = 0;
	
/*	if (pos == 0)
	{
		SEG_update_shift_regs(singles, pos);
		pos = 1;
	}
	else
	{
		SEG_update_shift_regs(decimals, pos);
		pos = 0;
	}*/

	SEG_update_shift_regs(singles, pos);
}

/**
 * ISR starts when Timer/Counter1 overflows. Increment decimal counter
 */
ISR(TIMER1_OVF_vect)
{
	singles++;
	if (singles > 5)
	{	
		singles = 0;
		decimals++;
		if (decimals > 5)
		{
			decimals = 0;
		}
	}   
}