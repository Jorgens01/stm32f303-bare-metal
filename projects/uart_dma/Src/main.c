/***************************************************************************
 * File name     :  main.c
 * Description   :  Main application file for an STM32F303 microcontroller.
 *                  This program demonstrates UART serial communication using DMA
 *                  for efficient data transmission. It configures GPIOA pin 5 (PA5)
 *                  to control an LED, initializes UART3, and uses DMA1 Channel 2
 *                  to transfer a string to the UART transmit data register.
 *                  The LED is toggled upon completion of the DMA transfer.
 *
 * Author        :  Jere Piirainen
 * Date          :  2025-06-17
 **************************************************************************/

#include <stdint.h>
#include "stm32f3xx.h"
#include "uart.h"

#define GPIOAEN			(1U << 17)  // Clock enable bit for GPIOA in RCC_AHBENR
#define LED_PIN         (1U << 5)   // PA5

/* --- Static function prototype local to this file --- */
static void dma_Callback(void);


int main(void)
{
	char message[31] = "Hello from STM32 DMA transfer\n\r";

	/* Enable clock access to GPIOA */
	RCC->AHBENR |= GPIOAEN;

	/* Set PA5 as output pin (01) */
	GPIOA->MODER |= (1U << 10);  // Set bit 10
    GPIOA->MODER &= ~(1U << 11); // Clear bit 11

    /* Initialize USART3 */
	uart3_tx_rx_init();

    /* Initialize USART3 for transmit and receive functionality */
	dma1Channel2Init((uint32_t)&message[0], (uint32_t)&USART3->TDR, 31);

    /* Kick-off the DMA transfer */
	USART3->TDR = message[0]; // Write the first byte to start the chain.

    return 0;
}


/**
 * @brief Callback function executed when the DMA transfer completes.
 * This function is called from the DMA interrupt service routine.
 */
static void dma_Callback(void)
{
	GPIOA->ODR |= LED_PIN;
}

/**
 * @brief DMA1 Channel 2 Interrupt Service Routine (ISR).
 * This function is automatically called by the MCU when a DMA interrupt occurs
 * for Channel 2, typically signaling a transfer complete event.
 */
void DMA1_CH2_IRQHandler(void)
{
	/* Check for transfer complete interrupt */
	if (DMA1->ISR & DMA1_ISR_TCIF2) {
		/* Clear flag */
		DMA1->IFCR |= DMA1_IFCR_CTCIF2;

		/* Do something */
		dma_Callback();
	}
}