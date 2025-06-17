/***************************************************************************
 * File name     :  uart.h
 * Description   :  Header file for the UART3 driver module. Provides functions
 *                  for initializing UART3 (TX/RX) and configuring DMA1
 *                  Channel 2 for UART transmit operations.
 *
 * Author        :  Jere Piirainen
 * Date          :  2025-06-17
 **************************************************************************/

#ifndef UART_H_
#define UART_H_
#include "stm32f3xx.h"


/* --- GPIO and USART Clock Enable Defines --- */
#define GPIOBEN        (1U << 18)  // Clock enable bit for GPIOB in RCC_AHBENR
#define USART3EN       (1U << 18)  // Clock enable bit for USART3 in RCC_APB1ENR

/* --- USART Control Register 1 (CR1) Bit Defines --- */
#define CR1_TE          (1U << 3)  // Transmitter Enable bit
#define CR1_RE          (1U << 2)  // Receiver Enable bit
#define CR1_UE          (1U << 0)  // USART Enable bit
#define CR1_RXNEIE      (1U << 5)  // RXNE Interrupt Enable (for receive interrupt)

/* --- USART Interrupt and Status Register (ISR) Bit Defines --- */
#define ISR_TXE         (1U << 7)  // Transmit data register empty flag
#define ISR_RXNE        (1U << 5)  // Read data register not empty flag (data ready to be read)

/* --- USART Control Register 3 (CR3) Bit Defines --- */
#define USART3_CR3_DMAT (1U << 7)  // DMA Enable Transmitter bit (for UART TX via DMA)

/* --- DMA Control Register (CCR) Bit Defines (for DMA1_Channel2->CCR) --- */
#define DMA1_CCR_EN          (1U << 0)   // DMA Channel Enable bit
#define DMA1_MINC            (1U << 7)   // Memory Increment Mode Enable
#define DMA1_DIR             (1U << 4)   // Transfer Direction (0: Periph to Mem; 1: Mem to Periph)
#define DMA1_CCR_TCIE        (1U << 1)   // Transfer Complete Interrupt Enable

/* --- DMA Interrupt and Status Register (ISR) and Clear Flag Register (IFCR) Defines --- */
#define DMA1_ISR_TCIF2      (1U << 5)   // Transfer Complete Flag for Channel 2 in DMA_ISR
#define DMA1_IFCR_CTCIF2    (1U << 5)   // Clear Transfer Complete Flag for Channel 2 in DMA_IFCR

/* --- DMA Clock Enable Define (specific for RCC_AHBENR) --- */
//#define RCC_AHBENR_DMA1EN   (1U << 0)   // Clock enable bit for DMA1 in RCC_AHBENR

/* --- System and UART Configuration Constants --- */
#define SYS_FREQ       8000000          // System clock frequency (e.g., 8 MHz HSI)
#define APB1_CLK       SYS_FREQ         // APB1 bus clock frequency (assuming no prescaler for APB1)

#define UART_BAUDRATE  115200           // Desired UART Baud rate
 

/**
 * @brief Initializes USART3 for both transmit (TX) and receive (RX) functionality.
 * Configures GPIO pins PB10 (TX) and PB11 (RX) for Alternate Function 7 (AF7),
 * enables clocks, sets baud rate, and enables the UART module.
 */
void uart3_tx_rx_init(void);

/**
 * @brief Initializes DMA1 Channel 2 for a memory-to-peripheral transfer.
 * This function sets up the source, destination, length, and transfer direction,
 * and enables the DMA channel and its transfer complete interrupt.
 * It's typically used for UART Transmit via DMA.
 * @param src The starting address of the source data in memory.
 * @param dst The starting address of the destination peripheral register (e.g., USARTx->TDR).
 * @param len The number of data transfers to be performed.
 */
void dma1Channel2Init(uint32_t src, uint32_t dst, uint32_t len);

/**
 * @brief Reads a single character from the USART3 receive data register.
 * This function blocks until data is available in the receive buffer.
 * @return The character received from USART3.
 */
char uart3_read(void);

/**
 * @brief Transmits a null-terminated string over USART3 using polling.
 * @param str Pointer to the constant null-terminated string to transmit.
 * @note This function is provided for basic string transmission without DMA.
 */
void uart3_puts(const char *str);
 
#endif /* UART_H_ */