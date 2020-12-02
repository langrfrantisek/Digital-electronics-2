/*
 * DAC.c
 *
 * Created: 26.11.2020 21:39:16
 * Author : Fraja
 */ 

/* Defines -----------------------------------------------------------*/
#define pin0 PB2      
#define pin1 PB3
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
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function
#include "uart.h"           // Peter Fleury's UART library

#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay func
#endif

int main(void)
{
    //Configure outputs for 8-bit R-2R DAC
    GPIO_config_output(&DDRB, pin0);
    GPIO_config_output(&DDRB, pin1);
    GPIO_config_output(&DDRD, pin2);
    GPIO_config_output(&DDRD, pin3);
    GPIO_config_output(&DDRD, pin4);
    GPIO_config_output(&DDRD, pin5);
    GPIO_config_output(&DDRD, pin6);
    GPIO_config_output(&DDRD, pin7);
    
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(1, 0); 
    lcd_puts("val:");
    
    /* Configuration of 8-bit Timer/Counter0 */
    TIM0_overflow_16us();
    TIM0_overflow_interrupt_enable();
    
    // Initialize UART to asynchronous, 8N1, 9600
    uart_init(UART_BAUD_SELECT(9600, F_CPU));
    
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
    char value_string[2] = "  ";      // String for converting numbers by itoa()
    
    //Writing values to pins, where is the R-2R connected
    PORTD = value & 0b11111100;
    PORTB = (value & 0b00000011) << 2;
    
    //Triangle function    
    /*if (number_of_overflows < 256)
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
        //value = 0;
    }*/
    
    //Ramp function
    //value++;
    
    //square function
    /*if (number_of_overflows < 2)
    {
        value = 0b11111111;
    }
    else if (number_of_overflows < 4)
    {
        value = 0;
    }
    else
    {
        number_of_overflows = 0;
    }*/
       
    number_of_overflows++;
    
    //Writing UART and LCD 
    /*if (number_of_overflows == 10)
    {
        itoa(value, value_string, 10);
        lcd_gotoxy(1, 1);
        lcd_puts(value_string);
        uart_puts(value_string);
        uart_puts("\r\n");
    }*/
    
    
}