/***********************************************************************
 * 
 * Digital electronics 2 BUT
 * Project application of analog signal generator using 8-bit R-2R DAC
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 * 
 * Frantisek Langr ID: 203514
 * 15.12.2020
 *
 **********************************************************************/ 

/* Defines -----------------------------------------------------------*/
#define pin0 PB2      
#define pin1 PB3
#define pin2 PD2
#define pin3 PD3
#define pin4 PD4
#define pin5 PD5
#define pin6 PD6
#define pin7 PD7

#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay func
#endif

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "gpio.h"           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function
#include "uart.h"           // Peter Fleury's UART library

volatile uint8_t btn_pressed = 11;  // remember last pressed button, 11 by default means 0 pressed (output off)
volatile uint8_t freq = 1;          // variable used for changing frequency, 1 by default means one step between values

// custom characters definitions for LCD - function symbols
uint8_t customChar[32] = {
    // sine 
    0b00000, 0b01000, 0b10100, 0b10100, 0b00101, 0b00101, 0b00010, 0b00000,
    // rectangle
    0b00000, 0b00000, 0b01110, 0b01010, 0b01010, 0b11011, 0b00000, 0b00000,
    // triangle
    0b00000, 0b00000, 0b00100, 0b01010, 0b10001, 0b00000, 0b00000, 0b00000,
    // ramp
    0b00000, 0b00001, 0b00011, 0b00101, 0b01001, 0b10001, 0b00000, 0b00000
};

// look up table for sine wave from http://aquaticus.info/pwm-sine-wave 
const uint8_t  sine_wave[256] = {
    0x80, 0x83, 0x86, 0x89, 0x8C, 0x90, 0x93, 0x96,
    0x99, 0x9C, 0x9F, 0xA2, 0xA5, 0xA8, 0xAB, 0xAE,
    0xB1, 0xB3, 0xB6, 0xB9, 0xBC, 0xBF, 0xC1, 0xC4,
    0xC7, 0xC9, 0xCC, 0xCE, 0xD1, 0xD3, 0xD5, 0xD8,
    0xDA, 0xDC, 0xDE, 0xE0, 0xE2, 0xE4, 0xE6, 0xE8,
    0xEA, 0xEB, 0xED, 0xEF, 0xF0, 0xF1, 0xF3, 0xF4,
    0xF5, 0xF6, 0xF8, 0xF9, 0xFA, 0xFA, 0xFB, 0xFC,
    0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFD,
    0xFD, 0xFC, 0xFB, 0xFA, 0xFA, 0xF9, 0xF8, 0xF6,
    0xF5, 0xF4, 0xF3, 0xF1, 0xF0, 0xEF, 0xED, 0xEB,
    0xEA, 0xE8, 0xE6, 0xE4, 0xE2, 0xE0, 0xDE, 0xDC,
    0xDA, 0xD8, 0xD5, 0xD3, 0xD1, 0xCE, 0xCC, 0xC9,
    0xC7, 0xC4, 0xC1, 0xBF, 0xBC, 0xB9, 0xB6, 0xB3,
    0xB1, 0xAE, 0xAB, 0xA8, 0xA5, 0xA2, 0x9F, 0x9C,
    0x99, 0x96, 0x93, 0x90, 0x8C, 0x89, 0x86, 0x83,
    0x80, 0x7D, 0x7A, 0x77, 0x74, 0x70, 0x6D, 0x6A,
    0x67, 0x64, 0x61, 0x5E, 0x5B, 0x58, 0x55, 0x52,
    0x4F, 0x4D, 0x4A, 0x47, 0x44, 0x41, 0x3F, 0x3C,
    0x39, 0x37, 0x34, 0x32, 0x2F, 0x2D, 0x2B, 0x28,
    0x26, 0x24, 0x22, 0x20, 0x1E, 0x1C, 0x1A, 0x18,
    0x16, 0x15, 0x13, 0x11, 0x10, 0x0F, 0x0D, 0x0C,
    0x0B, 0x0A, 0x08, 0x07, 0x06, 0x06, 0x05, 0x04,
    0x03, 0x03, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03,
    0x03, 0x04, 0x05, 0x06, 0x06, 0x07, 0x08, 0x0A,
    0x0B, 0x0C, 0x0D, 0x0F, 0x10, 0x11, 0x13, 0x15,
    0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24,
    0x26, 0x28, 0x2B, 0x2D, 0x2F, 0x32, 0x34, 0x37,
    0x39, 0x3C, 0x3F, 0x41, 0x44, 0x47, 0x4A, 0x4D,
    0x4F, 0x52, 0x55, 0x58, 0x5B, 0x5E, 0x61, 0x64,
    0x67, 0x6A, 0x6D, 0x70, 0x74, 0x77, 0x7A, 0x7D
};

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
    
    // Set pointer to beginning of CGRAM memory
    lcd_command(1 << LCD_CGRAM);
    for (uint8_t i = 0; i < 8*4; i++)
    {
        // Store all new chars to memory line by line
        lcd_data(customChar[i]);
    }
    // Set DDRAM address
    lcd_command(1 << LCD_DDRAM);
    
    //Put signal options at LCD display
    lcd_gotoxy(1, 0); 
    lcd_puts("Choose signal:");
    lcd_gotoxy(1, 1); 
    lcd_puts("1");
    lcd_putc(0);
    lcd_puts("  2");
    lcd_putc(1);
    lcd_puts("  3");
    lcd_putc(2);
    lcd_puts("  4");
    lcd_putc(3);
   
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
   
    /* Configuration of Timer/Counter0 */
    TIM0_overflow_16us();
    TIM0_overflow_interrupt_enable();
    
    /* Configuration of Timer/Counter1 */
    TIM1_overflow_4ms();
    TIM1_overflow_interrupt_enable();
    
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
/**
 * ISR calculate and write value to R2-R. 
 */
ISR(TIMER0_OVF_vect)
{    
	static int16_t number_of_overflows = 0;     // counting number of overflows 
    static uint8_t lookup_number = 0;           // position in lookup table
    static uint8_t value = 0;                   // this value is writen to R2-R

    // Writing values to pins, where the R-2R is connected
    PORTD = value & 0b11111100;
    PORTB = (value & 0b00000011) << 2;
  
    // if 1 is pressed: Sine function
    if (btn_pressed == 1)
    {
        // write samples of sine wave to value, freq defines step between samples
        value = sine_wave[lookup_number];
        lookup_number += freq;
    }
    
    // if 2 is pressed: Square function
    if (btn_pressed == 2)
    {
        // first 128 cycles write log. 1 to output value, another 128 cycles write log. 0
        if (number_of_overflows < 128)
        {
            value = 255;
        }
        else if (number_of_overflows < 256)
        {
            value = 0;
        }
        else
        {
            //after one period reset number of overflows
            number_of_overflows = 0;
        }
    }
    
    // if 3 is pressed: Triangle function
    if (btn_pressed == 3)
    {
        // first 256 cycles increase output value, another 256 cycles decrease output value, freq defines step between samples
        if (number_of_overflows < 256 && value >= 0)
        {        
            value += freq;
        } 
        else if (number_of_overflows < 511 && value > 0)
        {
            value -= freq;
        }
        else 
        {
            //after one period reset number of overflows
            number_of_overflows = 0;
            value = 0;
        }
    }
      
    // if 4 is pressed: Ramp function    
    if (btn_pressed == 4)
    {
        // first 256 cycles increase output value, after that value overflows, freq defines step between samples
        value += freq;
    }
    
    // increase number of overflows with freq step   
    number_of_overflows += freq;   
  
}
/* -------------------------------------------------------------------*/
/**
 * ISR start ADC conversion. 
 */
ISR(TIMER1_OVF_vect)
{
   ADCSRA |= (1 << ADSC);   
}
/* -------------------------------------------------------------------*/
/**
 * ISR starts when ADC completes the conversion. 
 * ISR identify pressed button, write on UART and LCD
 */

ISR(ADC_vect)
{
    static uint16_t ADC_value = 0;          // variable for ADC value
    static uint8_t btn_pressed_next = 11;   // used when freq is changed
    static uint8_t freq_control = 0;        // make sure that freq is changed only once per button press 
    static uint8_t freq_next = 1;           // make sure that freq is changed only once per button press 
    static uint8_t control = 0;             // write menu when button is pressed
    static uint16_t f_Hz = 0;               // frequency in Hz
    static uint16_t ADC_value_previous = 0; // previous ADC value
    char data[10];                          // data string for UART
    
    // write ADC value to ADC_value
    ADC_value = ADC;
    
    // only when ADC_value is changed != previous value
    if (ADC_value != ADC_value_previous)
    {
        // change previous ADC value to new one
        ADC_value_previous = ADC_value;
        
        // ADC value < 5 means button 1 pressed
        if (ADC_value < 5)
        {
            btn_pressed = 1;
            // calculate frequency
            if (btn_pressed == 3) f_Hz = (((1)/(16e-6))/(258))/2;
            else f_Hz = ((1)/(16e-6))/(256);
            // Convert to string in decimal
            itoa(f_Hz*freq, data, 10);
            // write on UART
            uart_puts("Sine");
            uart_puts(" f = ");
            uart_puts(data);
            uart_puts(" Hz");
            // write on LCD
            lcd_gotoxy(8, 0);
            lcd_putc('1');
            lcd_putc(0);
            lcd_gotoxy(10, 1);
            lcd_puts("    ");
            lcd_gotoxy(10, 1);
            lcd_puts(data);
        }
        else if (ADC_value > 97 && ADC_value < 107)
        {
            btn_pressed = 2;
            // calculate frequency
            if (btn_pressed == 3) f_Hz = (((1)/(16e-6))/(258))/2;
            else f_Hz = ((1)/(16e-6))/(256);
            // Convert to string in decimal
            itoa(f_Hz*freq, data, 10);
            // write on UART
            uart_puts("Square");
            uart_puts(" f = ");
            uart_puts(data);
            uart_puts(" Hz");
            // write on LCD
            lcd_gotoxy(8, 0);
            lcd_putc('2');
            lcd_putc(1);
            lcd_gotoxy(10, 1);
            lcd_puts("    ");
            lcd_gotoxy(10, 1);
            lcd_puts(data);
        }
        else if (ADC_value > 180 && ADC_value < 190)
        {
            btn_pressed = 3;
            // calculate frequency
            if (btn_pressed == 3) f_Hz = (((1)/(16e-6))/(258))/2;
            else f_Hz = ((1)/(16e-6))/(256);
            // Convert to string in decimal
            itoa(f_Hz*freq, data, 10);
            // write on UART
            uart_puts("Triangle");
            uart_puts(" f = ");
            uart_puts(data);
            uart_puts(" Hz");
            // write on LCD
            lcd_gotoxy(8, 0);
            lcd_putc('3');
            lcd_putc(2);
            lcd_gotoxy(10, 1);
            lcd_puts("    ");
            lcd_gotoxy(10, 1);
            lcd_puts(data);
        }
        else if (ADC_value > 250 && ADC_value < 260)
        {
            btn_pressed = 4;
            // calculate frequency
            if (btn_pressed == 3) f_Hz = (((1)/(16e-6))/(258))/2;
            else f_Hz = ((1)/(16e-6))/(256);
            // Convert to string in decimal
            itoa(f_Hz*freq, data, 10);
            // write on UART
            uart_puts("Ramp");
            uart_puts(" f = ");
            uart_puts(data);
            uart_puts(" Hz");
            // write on LCD
            lcd_gotoxy(8, 0);
            lcd_putc('4');
            lcd_putc(3);
            lcd_gotoxy(10, 1);
            lcd_puts("    ");
            lcd_gotoxy(10, 1);
            lcd_puts(data);
        }
        else if (ADC_value > 506 && ADC_value < 516 && freq > 1)    //*
        {
            btn_pressed_next = 10;
            // calculate frequency
            if (btn_pressed == 3) f_Hz = (((1)/(16e-6))/(258))/2;
            else f_Hz = ((1)/(16e-6))/(256);   
            // Convert to string in decimal    
            itoa(f_Hz*(freq-1), data, 10);
            // write on UART
            uart_puts("f--");
            uart_puts("\r\n");
            uart_puts("f = ");
            uart_puts(data);
            uart_puts(" Hz");
            // write on LCD
            lcd_gotoxy(10, 1);
            lcd_puts("    ");
            lcd_gotoxy(10, 1);
            lcd_puts(data);
            
        }
        else if (ADC_value > 533 && ADC_value < 543)    //0
        {
            btn_pressed = 11;
            // write on UART
            uart_puts("Output off");
        }
        else if (ADC_value > 557 && ADC_value < 567)    //#
        {
            btn_pressed_next = 12;
            // calculate frequency
            if (btn_pressed == 3) f_Hz = (((1)/(16e-6))/(258))/2;
            else f_Hz = ((1)/(16e-6))/(256); 
            // Convert to string in decimal 
            itoa(f_Hz*(freq+1), data, 10);
            // write on UART
            uart_puts("f++");
            uart_puts("\r\n");
            uart_puts("f = ");
            uart_puts(data);
            uart_puts(" Hz");
            // write on LCD
            lcd_gotoxy(10, 1);
            lcd_puts("    ");
            lcd_gotoxy(10, 1);
            lcd_puts(data);
        }
        else if (ADC_value > 1017)
        {
            // write on UART
            uart_puts("\r\n"); 
        }     
    }

    if (ADC_value > 1017)
    {
        // nothing pressed, freq_control make sure that freq is changed only once per button press
        freq_control++;
    }

    // show pressed menu
    if (btn_pressed != 11 && control == 0)
    {
        // write on LCD
        lcd_gotoxy(0, 0);
        lcd_puts("Pressed:   Exit0");
        lcd_gotoxy(0, 1);
        lcd_puts("*f-- #f++     Hz");
        control = 1;
    }
    // * pressed, decrease freq
    if (btn_pressed_next == 10)
    {
        btn_pressed_next = btn_pressed;
        if (freq_next == freq && freq > 1)
        {
            freq_next = freq - 1;
        }
    }
    // 0 pressed, Output off and reset menu
    else if (btn_pressed == 11)
    {
        // write on LCD
        lcd_gotoxy(0, 0);
        lcd_puts(" Choose signal: ");
        lcd_gotoxy(0, 1);
        lcd_puts(" 1");
        lcd_putc(0);
        lcd_puts("  2");
        lcd_putc(1);
        lcd_puts("  3");
        lcd_putc(2);
        lcd_puts("  4");
        lcd_putc(3);
        lcd_putc(' ');
    
        control = 0;
        freq_next = 1;
        freq = 1;
        
    } 
    
    // # pressed, increase freq
    else if (btn_pressed_next == 12)
    {
        btn_pressed_next = btn_pressed;
        if (freq_next == freq)
        {
            freq_next = freq + 1;
        }
    }
    
    // make sure that freq is changed only once per button press
    if (freq_control >= 10)
    {
        freq = freq_next;
        freq_control = 0;
    }
}