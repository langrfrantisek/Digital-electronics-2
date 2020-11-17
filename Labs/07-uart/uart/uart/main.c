/***********************************************************************
 * 
 * Analog-to-digital conversion with displaying result on LCD and 
 * transmitting via UART.
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
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function
#include "uart.h"           // Peter Fleury's UART library

#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay func
#endif

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Use Timer/Counter1
 * and start ADC conversion four times per second. Send value to LCD
 * and UART.
 */
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(1, 0); lcd_puts("val:");   

    // Configure ADC to convert PC0[A0] analog value
    // Set ADC reference to AVcc
    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 << REFS1);
    
    // Set input channel to ADC0
    ADMUX &= ~(1 << MUX3);
    ADMUX &= ~(1 << MUX2);
    ADMUX &= ~(1 << MUX1);
    ADMUX &= ~(1 << MUX0);
    
    // Enable ADC module
    ADCSRA |= (1 << ADEN);
    
    // Enable conversion complete interrupt
    ADCSRA |= (1 << ADIE);
    
    // Set clock prescaler to 128
    ADCSRA |= (1 << ADPS2);
    ADCSRA |= (1 << ADPS1);
    ADCSRA |= (1 << ADPS0);

    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Enable interrupt and set the overflow prescaler to 262 ms   
    TIM1_overflow_262ms();
    TIM1_overflow_interrupt_enable();
    
    // Initialize UART to asynchronous, 8N1, 9600
    uart_init(UART_BAUD_SELECT(9600, F_CPU));

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
 * ISR starts when Timer/Counter1 overflows. Use single conversion mode
 * and start conversion four times per second.
 */
ISR(TIMER1_OVF_vect)
{
    // Start ADC conversion
    ADCSRA |= (1 << ADSC);
}

/* -------------------------------------------------------------------*/
/**
 * ISR starts when ADC completes the conversion. Display value on LCD
 * and send it to UART.
 */
ISR(ADC_vect)
{
    uint16_t value = ADC;    // Copy ADC result to 16-bit variable
    char lcd_string[10] = "          ";
    char one[1] = "1";
    uint8_t num_of_ones = 0;

    //Clear positions
    lcd_gotoxy(6, 0);
    lcd_puts(lcd_string);

    //Print ADC value on LED in binary
    itoa(value, lcd_string, 2);    // Convert to string in binary
    lcd_gotoxy(6, 0);
    lcd_puts(lcd_string);
    
    //send data through UART
    uart_puts("ADC value in binary: ");
    uart_puts(lcd_string);
    uart_puts("\r\n");
        
    //Clear parity positions
    lcd_gotoxy(1, 1);
    lcd_puts("               ");
    
    //calculate number of ones in lcd_string
    for (uint8_t i = 0; i < sizeof(lcd_string)/sizeof(char); i++)
    {
        if (lcd_string[i] == one[0])
        {
            num_of_ones++;
        }
    }
    
    //set parity position
    lcd_gotoxy(1, 1);
    
    //calculate parity 
    if (num_of_ones % 2 == 0)
    {        
         lcd_puts("even: 0 odd: 1");    //display parity on lcd  
         uart_puts("even: 0 odd: 1");   //send data through UART   
    }
    else 
    {
         lcd_puts("even: 1 odd: 0");    //display parity on lcd
         uart_puts("even: 1 odd: 0");   //send data through UART
    }
    uart_puts("\r\n");
}
