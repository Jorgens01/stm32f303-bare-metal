/***************************************************************************
 * File name     :      main.c
 * Description   :      Main application file for an STM32F3 microcontroller.
 *                      This program initializes UART3 for serial communication and
 *                      configures GPIOA pin 5 (PA5) to control an LED. It then
 *                      continuously transmits a message over UART and toggles the LED
 *                      every 2 seconds using a SysTick-based delay.
 *
 * Author        :      Jere Piirainen
 * Date          :      2025-06-16
 **************************************************************************/
#include "stm32f3xx.h"
#include "systick.h"
#include "uart.h"

/* --- GPIOA defines --- */
#define GPIOAEN     (1U << 17)  // Clock enable bit for GPIOA in RCC_AHBENR
#define LED_PIN     (1U << 5)   // PA5

int main(void)
{
    /* Enable clock access to GPIOA */
    RCC->AHBENR |= GPIOAEN;

    /* Set PA5 as output pin (01) */
    /* MODER bits for PA5 are MODER5[1:0] (bits 11:10) */
    GPIOA->MODER |= (1U << 10);
    GPIOA->MODER &= ~(1U << 11);

    const char *string = "2 seconds has passed...\r\n"; // Message to be transmitted

    /* Initialize USART3 for transmit and receive functionality */
    uart3_tx_rx_init();

    /* Main loop */
    while (1) {
        /* Transmit message over UART3 */
        uart3_puts(string);

        /* Toggle LED */
        GPIOA->ODR ^= LED_PIN;

        /* Introduce a 2000 ms (2 second) delay using SysTick timer */
        systickDelayMs(2000);
    }
}
