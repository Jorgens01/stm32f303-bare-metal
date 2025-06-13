/***************************************************************************
 * File name    :   main.c
 * Description  :   This file contains the main application logic for
 *                  controlling an LED connected to PA5 via UART commands.
 *                  It receives '1' to turn the LED on and '0' to turn it off,
 *                  providing feedback to the terminal only when the LED state
 *                  changes.
 * 
 * Author       :   Jere Piirainen
 * Date         :   2025-06-13
 **************************************************************************/

#include "stm32f3xx.h"
#include "uart.h"

#define GPIOAEN     (1U << 17)
#define LED_PIN     (1U << 5)   // PA5


int main(void)
{
    char key;

    /* Enable clock access to GPIOA */
    RCC->AHBENR |= GPIOAEN;

    /* Set PA5 as an output pin */
    GPIOA->MODER |= (1U << 10);
    GPIOA->MODER &= ~(1U << 11);

    /* Initialize UART for both transmission and reception */
    uart3_tx_rx_init();

    /* Inform user about the expected input */
    uart3_puts("Send '1' to turn LED on, '0' to turn it off.\r\n");

    
    while (1) {
        key = uart3_read();  // Read a character from UART

        /* Check for '1' and if the LED is currently off */
        if ( (key == '1') && !(GPIOA->ODR & LED_PIN) ) {
            GPIOA->ODR |= LED_PIN;                      // PA5 HIGH
            uart3_puts("LED ON\r\n");                   // Confirm state change to terminal
        } 
        /* Check for '0' and if the LED is currently on */
        else if ( (key == '0') && (GPIOA->ODR & LED_PIN) ) {
            GPIOA->ODR &= ~LED_PIN;                     // PA5 LOW
            uart3_puts("LED OFF\r\n");                  // Confirm state change to terminal
        } 
        /* Handle invalid input or redundant commands */
        else {
            uart3_puts("Invalid input. Send '1' or '0'.\r\n");
        }
    }
}
