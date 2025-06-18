/***************************************************************************
 * File name     :  i2c.h
 * Description   :  Header file for the I2C1 driver module.
 *                  Provides function prototypes and defines for configuring and
 *                  communicating via I2C1, specifically for byte and burst read/write
 *                  operations with slave devices. It defines pinouts, clock enables,
 *                  and I2C register bit masks.
 *
 * Author        :  Jere Piirainen
 * Date          :  2025-06-18
 **************************************************************************/
#ifndef I2C_H_
#define I2C_H_
#include "stm32f3xx.h"

 // --- Peripheral Clock Enable Defines ---
#define GPIOB_EN       (1U << 18)  // Clock enable bit for GPIOB in RCC_AHBENR register
#define I2C1_EN        (1U << 21)  // Clock enable bit for I2C1 in RCC_APB1ENR register

// --- I2C Control Register 1 (CR1) Bit Defines ---
#define CR1_PE         (1U << 0)   // Peripheral Enable bit in I2C_CR1

// --- I2C Timing Register (TIMINGR) Configuration Values ---
// These values are specific to achieve a 100kHz I2C clock speed with an 8MHz PCLK.
// They are combined into the TIMINGR register during initialization.
#define I2C1_SCLL      (0x13U << 0)    // SCL low period (Master mode)
#define I2C1_SCLH      (0xFU << 8)     // SCL high period (Master mode)
#define I2C1_SDADEL    (0x2U << 16)    // SDA data hold up time
#define I2C1_SCLDEL    (0x4U << 20)    // SCL data setup time
#define I2C1_PRESC     (0x1U << 28)    // Prescaler value

// --- I2C Interrupt and Status Register (ISR) Flags ---
// These bits indicate the current status or events during I2C transfers.
#define ISR_BUSY       (1U << 15)  // Bus busy flag (1=bus is busy)
#define ISR_ADDR       (1U << 3)   // Address matched flag (slave mode: own address matched)
#define ISR_TXIS       (1U << 1)   // Transmit Interrupt Status (transmit data register empty)
#define ISR_TC         (1U << 6)   // Transfer Complete (NBYTES reached, AUTOEND=0)
#define ISR_TCR        (1U << 7)   // Transfer Complete Reload (NBYTES reached, AUTOEND=0, RELOAD=1)
#define ISR_RXNE       (1U << 2)   // Receive data register not empty (data ready to be read)
#define ISR_NACKF      (1U << 4)   // Not Acknowledge received flag
#define ISR_STOPF      (1U << 5)   // STOP detection flag

// --- I2C Control Register 2 (CR2) Bit Defines ---
// These bits control the generation of start/stop conditions, transfer direction, etc.
#define CR2_START      (1U << 13)  // Generate START condition
#define CR2_STOP       (1U << 14)  // Generate STOP condition
#define CR2_RD_WRN     (1U << 10)  // Transfer direction (0: Write, 1: Read)
#define CR2_NBYTES_POS 16          // Bit position for NBYTES field (number of bytes to transfer)
#define CR2_RELOAD     (1U << 24)  // RELOAD bit (for transfers > 255 bytes)
#define CR2_AUTOEND    (1U << 25)  // Automatic END mode (generates STOP and NACK on last byte)

// --- I2C Interrupt Clear Register (ICR) Flags ---
// Writing a '1' to these bits clears the corresponding flag in the ISR.
#define ICR_STOPCF     (1U << 5)   // Clear STOP detection flag
#define ICR_NACKCF     (1U << 4)   // Clear NACK received flag


/**
 * @brief Initializes the I2C1 peripheral.
 * Configures GPIO pins for I2C (PB8 SCL, PB9 SDA), enables clocks,
 * and sets up I2C timing for standard mode (100kHz).
 */
void I2C1_Init(void);


/**
 * @brief Reads a single byte from an I2C slave device.
 * Performs a write operation to send the memory address, followed by a
 * restart and a read operation to retrieve the data byte.
 * @param saddr   7-bit slave address.
 * @param maddr   8-bit memory address within the slave device to read from.
 * @param data    Pointer to a uint8_t variable where the read byte will be stored.
 */
void I2C1_ByteRead(char saddr, char maddr, uint8_t *data);


/**
 * @brief Performs a sequential (burst) read of multiple bytes from an I2C slave.
 * Sends the starting memory address, then reads 'n' consecutive bytes.
 * This function handles up to 255 bytes in a single transfer.
 * @param saddr   7-bit slave address.
 * @param maddr   8-bit starting memory address within the slave device.
 * @param n       Number of bytes to read.
 * @param data    Pointer to a uint8_t array where the read bytes will be stored.
 */
void I2C1_BurstRead(char saddr, char maddr, int n, uint8_t *data);


/**
 * @brief Performs a sequential (burst) write of multiple bytes to an I2C slave.
 * Sends the starting memory address, then writes 'n' consecutive bytes.
 * This function handles up to 254 data bytes plus the memory address in a single transfer.
 * @param saddr   7-bit slave address.
 * @param maddr   8-bit starting memory address within the slave device.
 * @param n       Number of bytes to write.
 * @param data    Pointer to a uint8_t array containing the bytes to be written.
 */
void I2C1_BurstWrite(char saddr, char maddr, int n, uint8_t *data);


/**
 * @brief I2C1 Pinouts
 *
 * This section defines the GPIO pins used for I2C1 communication.
 * - PB8: I2C1_SCL (Serial Clock Line)
 * - PB9: I2C1_SDA (Serial Data Line)
 * These pins must be configured for Alternate Function mode (AF4 for I2C1)
 * and Open-Drain output type with Pull-up resistors.
 */

 #endif /* I2C_H_ */