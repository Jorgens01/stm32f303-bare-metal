/***************************************************************************
 * File name    :   main.c
 * Description  :   This file contains the main function to toggle an LED
 *                  connected to pin PA5 of an STM32F3 microcontroller.
 *                  The LED will toggle on and off with a simple delay loop.
 * 
 * Author       :   Jere Piirainen
 * Date         :   2025-06-05
 **************************************************************************/

#include "stm32f3xx.h"  // Header file

#define GPIOAEN     (1U << 17)
#define LED_PIN     (1U << 5)  // PA5

int main(void)
{
    /* Enable clock access to GPIOA */
    RCC->AHBENR |= GPIOAEN;

    /* Make pin 5 output */
    // bits 11:10 to be set to 01 (output mode)
    GPIOA->MODER |= (1U << 10);  // Set PA5 as output
    GPIOA->MODER &= ~(1U << 11); // Clear PA5 mode bits

    /* Main loop */
    while (1) {
        GPIOA->ODR ^= LED_PIN;  // Toggle PA5 (LED)
        for (volatile int i = 0; i < 1000000; i++) {}    // Simple delay loop
    }
}
