/***************************************************************************
 * File name    :   main.c
 * Description  :   This file demonstrates basic GPIO input and output on 
 *                  an STM32F303 microcontroller. It configures GPIOA Pin 5 
 *                  as an output (for an LED) and GPIOC Pin 13 as an input 
 *                  (for a user button), allowing the button press to control 
 *                  the LED's state.
 * 
 * Author       :   Jere Piirainen
 * Date         :   2025-06-07
 **************************************************************************/

 #include "stm32f3xx.h"  // Header file

#define GPIOAEN     (1U << 17)
#define GPIOCEN     (1U << 19)

#define LED_PIN     (1U << 5)   // PA5
#define BTN_PIN     (1U << 13)  // PC13

int main(void)
{
    /* Enable clock access to GPIOA and GPIOC */
    RCC->AHBENR |= GPIOAEN;
    RCC->AHBENR |= GPIOCEN;

    /* Set PA5 as output pin */
    GPIOA->MODER |= (1U << 10);
    GPIOA->MODER &= ~(1U << 11);

    /* Set PC13 as input pin */
    GPIOC->MODER &= ~(1U << 26);
    GPIOC->MODER &= ~(1U << 27);

    while (1) {
        /* Check if BTN is pressed */
        if (GPIOC->IDR & BTN_PIN) {     // If button is not pressed
            GPIOA->BSRR = (1U << 21);   // Turn LED off by resetting the bit
        }
        else {  /* Turn on LED */
            GPIOA->BSRR = LED_PIN;  // Set the bit
        }
    }
}