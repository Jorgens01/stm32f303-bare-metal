/***************************************************************************
 * File name     :    uart.c
 * Description   :    This file provides functions to initialize and control
 *                    USART3 on an STM32F3 microcontroller for serial
 *                    communication (UART). It includes functions for
 *                    transmitting and receiving single characters,
 *                    and a utility for transmitting strings.
 *
 * Author        :    Jere Piirainen
 * Date          :    2025-06-13
 **************************************************************************/
#include "uart.h"
#include "stm32f3xx.h"

/* --- Peripheral base addresses and bit definitions --- */
#define GPIOBEN         (1U << 18)
#define USART3EN        (1U << 18)

#define CR1_TE          (1U << 3)       // Transmit enable bit in CR1
#define CR1_RE			(1U << 2)       // Receive enable bit in CR1

#define CR1_UE          (1U << 0)       // USART enable bit in CR1
#define ISR_TXE         (1U << 7)       // Transmit data register empty flag
#define ISR_RXNE		(1U << 5)	    // Read data register not empty flag

/* --- System and UART configuration constants --- */
#define SYS_FREQ       8000000          // System clock frequency (8 MHz)
#define APB1_CLK       SYS_FREQ         // APB1 bus clock frequency 

#define UART_BAUDRATE  115200           // Desired Baud rate


/* --- Static function prototypes (helper functions local to this file) --- */

static void uart_set_baudrate(USART_TypeDef *UARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);
static void uart3_write(int ch);


/* --- Printing helping functions --- */
/**
 * @brief Transmits a null-terminated string over USART3.
 * @param str Pointer to the constant null-terminated string to transmit.
 */
void uart3_puts(const char *str)
{
    /* Loop until the null terminator ('\0') is encountered */
    while (*str != '\0') {
        /* Transmit crrent character */
        uart3_write(*str);
        /* Move to the next character */
        str++;
    }
}

void uart3_put_int(int num)
{
    char buffer [12];       // Buffer to hold the string representation of the number
    int i = 0;              // Index for the buffer
    int is_negative = 0;    // Flag to track if the original number is negative
    
    /* Handle special case where number is 0 */
    if (num == 0) {
        uart3_write('0');
        return;
    }

    /* Handle negative numbers */
    if (num < 0) {
        is_negative = 1;    // Set the flag
        num = -num;         // Convert to positive for digit extraction
    }

    /* Extract digits in reverse order */
    while (num > 0) {
        /**
         * Get the last digit (num % 10).
         * Convert digit to its ASCII character representation.
         */
        buffer[i++] = (num % 10) + '0';

        /* Remove last digit from the number */
        num /= 10;
    }

    /* Add negative sign if the original number was negative */
    if (is_negative) {
        buffer[i++] = '-';
    }

    /* Reverse the string in the buffer */
    int start = 0;
    int end = i - 1;            // 'i' will now be total number of characters

    while (start < end) {
        /* Swap characters from the beginning and end */
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;

        /* Move pointers towards the center */
        start++;
        end--;
    }

    /* Null-terminate the string */
    buffer[i] = '\0';

    /* Send the formatted string via UART */
    uart3_puts(buffer);
}



/**
 * @brief Initializes USART3 for both transmit (TX) and receive (RX) functionality.
 * Configures GPIO pins PB10 (TX) and PB11 (RX) for Alternate Function 7 (AF7),
 * enables clocks, sets baud rate, and enables the UART module.
 */
void uart3_tx_rx_init(void)
{
    /********** 1. Configure UART GPIO pins **********/

    /* Enable clock access to gpiob */
    RCC->AHBENR |= GPIOBEN;

    /* Set PB10 (UART3_TX) mode to alternate function mode (10) */
    GPIOB->MODER &= ~(1U << 20);
    GPIOB->MODER |= (1U << 21);

    /* Set PB11 (UART3_RX) mode to alternate function mode (10) */
    GPIOB->MODER &= ~(1U << 22);
    GPIOB->MODER |= (1U << 23);


    /* Set PB10 alternate function type to UART_TX (AF7 = 0111) */
    GPIOB->AFR[1] |= (1U << 8);
    GPIOB->AFR[1] |= (1U << 9);
    GPIOB->AFR[1] |= (1U << 10);
    GPIOB->AFR[1] &= ~(1U << 11);

    /* Set PB11 alternate function type to UART_RX (AF7 = 0111) */
    GPIOB->AFR[1] |= (1U << 12);
    GPIOB->AFR[1] |= (1U << 13);
    GPIOB->AFR[1] |= (1U << 14);
    GPIOB->AFR[1] &= ~(1U << 15);


    /********** 2. Configure USART3 module **********/

    /* Enable clock access to USART3 */
    RCC->APB1ENR |= USART3EN;

    /* Configure baud rate */
    uart_set_baudrate(USART3, APB1_CLK, UART_BAUDRATE);

    /* Configure the transfer direction for both transmitter and receiver */
    USART3->CR1 = (CR1_TE |  CR1_RE);

    /* Enable uart module (Done AFTER all other configurations) */
    USART3->CR1 |= CR1_UE;
}



/**
 * @brief Reads a single character from the USART3 receive data register.
 * This function blocks until data is available in the receive buffer.
 * @return The character received from USART3.
 */
char uart3_read(void)
{
	/* Make sure transmit data register is NOT empty */
	while ( !(USART3->ISR & ISR_RXNE) ) {}      // Returns true if bit ISR_RXNE is set inside ISR register

	/* Read the data */
	return USART3->RDR;
}



/**
 * @brief Writes a single character to the USART3 transmit data register.
 * This function blocks until the transmit data register is empty,
 * indicating it's ready to accept new data.
 * @param ch The character to be transmitted.
 */
void uart3_write(int ch)
{
    /* Make sure transmit data register is empty */
    while ( !(USART3->ISR & ISR_TXE) ) {}      // Returns true if bit SR_TXE is set inside ISR register

    /* Write to transmit data register */
    USART3->TDR = (ch & 0xFF);
}



/**
 * @brief Sets the baud rate for the specified UART peripheral.
 * @param USARTx Pointer to the USART peripheral (e.g., USART3).
 * @param PeriphClk The peripheral clock frequency feeding the UART.
 * @param BaudRate The desired baud rate.
 */
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
	USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}



/**
 * @brief Computes the value for the USART Baud Rate Register (BRR).
 * This calculation performs integer division with rounding for simplicity.
 * For high precision, refer to the MCU's reference manual for fractional baud rate settings.
 * @param PeriphClk The peripheral clock frequency.
 * @param BaudRate The desired baud rate.
 * @return The calculated 16-bit value for the BRR register.
 */
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
    // Calculation: BRR = PeriphClk / BaudRate
    // Adding (BaudRate / 2U) before division performs rounding to the nearest integer.
    return ((PeriphClk + (BaudRate / 2U)) / BaudRate);
}
