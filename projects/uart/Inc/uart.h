/***************************************************************************
 * File name    :   uart.h
 * Description  :   Header file for the UART3 driver module. Provides functions
 *                  for initializing UART3 (TX/RX).
 * 
 * Author       :   Jere Piirainen
 * Date         :   2025-06-13
 **************************************************************************/

#ifndef UART_H_
#define UART_H_
 
void uart3_tx_rx_init(void);

char uart3_read(void);
void uart3_puts(const char *str);
 
#endif /* UART_H_ */