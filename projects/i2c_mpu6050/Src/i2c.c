/***************************************************************************
 * File name     :	i2c.c
 * Description   :	This file implements the I2C1 driver for STM32F3 microcontrollers.
 * 					It provides functions to initialize I2C1 and perform single byte
 * 					and multi-byte (burst) read/write operations with I2C slave devices.
 * 					The implementation uses polling for status flags and handles common
 * 					I2C transfer sequences like START, STOP, NACK, and data transfer.
 *
 * Author        :	Jere Piirainen
 * Date          :	2025-06-18
 **************************************************************************/
#include "i2c.h"

void I2C1_Init(void)
{
	/* Enable clock access to GPIOB */
	RCC->AHBENR |= GPIOB_EN;

	/* Set PB8 & PB9 mode to alternate function (10) */
	GPIOB->MODER &= ~((1U << 16) | (1U << 18));		// Clear bit 16 and 18
	GPIOB->MODER |= (1U << 17) | (1U << 19);		// Set bit 17 and 19

	/* Set PB8 & PB9 output type to open drain */
	GPIOB->OTYPER |= (1U << 8) | (1U << 9);

	/* Enable pull-up resistors for PB8 & PB9 (0b01)*/
	GPIOB->PUPDR |= (1U << 16) | (1U << 18);
	GPIOB->PUPDR &= ~((1U << 17) | (1U << 19));

	/* Set PB8 and PB9 alternate function type to I2C1 (AF4 = 0b0100) */
	// AFR[1] corresponds to AFRH for pins 8-15.
    // For PB8 (pin 8): AFRL/H index 0, bits [3:0]
    // For PB9 (pin 9): AFRL/H index 1, bits [7:4]
    // (4U << 0) sets AF4 for PB8; (4U << 4) sets AF4 for PB9
	GPIOB->AFR[1] |= (4U << 0) | (4U << 4); // AF4 for PB8 nad PB9

	/* Enable clock access to I2C1*/
	RCC->APB1ENR |= I2C1_EN;

	/* Disable I2C1 peripheral (PE bit) to allow configuration/reset */
	I2C1->CR1 &= ~CR1_PE;

	/* Configure I2C1 Timing Register (TIMINGR) for 100kHz clock speed */
    // These values are derived from the reference manual
    // based on PCLK1 frequency (8MHz) to achieve a 100kHz I2C clock.
	I2C1->TIMINGR = I2C1_SCLL | I2C1_SCLH | I2C1_SDADEL | I2C1_SCLDEL | I2C1_PRESC;

	/* Peripheral enable */
	I2C1->CR1 |= CR1_PE;
}


void I2C1_ByteRead(char saddr, char maddr, uint8_t* data)
{
	/* Wait until bus not busy */
	while (I2C1->ISR & ISR_BUSY) {}

	/* --- 1. Master transmitter, send memory address --- */
	/* Configure CR2 for slave address + write */
	I2C1->CR2 = 0;							// Clear CR2
	I2C1->CR2 |= (uint32_t)saddr << 1; 		// Slace address (7-bit)
	I2C1->CR2 &= ~CR2_RD_WRN;				// Write operation
	I2C1->CR2 |= (1U << CR2_NBYTES_POS);	// NBYTES = 1
	I2C1->CR2 &= ~CR2_AUTOEND;				// Software endmode (RESTART)

	/* Generate START condition */
	I2C1->CR2 |= CR2_START;

	/* Wait for TXIS is set (or NACKF) */
	while (!(I2C1->ISR & ISR_TXIS)) {
		if (I2C1->ISR & ISR_NACKF) {
			/* Handle NACK error */
			I2C1->ICR |= ICR_NACKCF;
			I2C1->CR2 |= CR2_STOP;
			while (!(I2C1->ISR & ISR_STOPF)) {}
			I2C1->ICR |= ICR_STOPCF;
			return;
		}
	}

	/* Write memory address to the transmit data register (TXDR) */
	I2C1->TXDR = maddr;

	/* Wait for TC flag, indicating maddr has been sent */
	while (!(I2C1->ISR & ISR_TC)) {}


	/* --- 2. read data from slave --- */
	/* Configure CR2 for slave address + read */
	I2C1->CR2 = 0;							// Clear CR2 before reconfiguring
	I2C1->CR2 |= (uint32_t)saddr << 1;		// Set 7-bit slave address
	I2C1->CR2 |= CR2_RD_WRN;				// Read operation
	I2C1->CR2 |= (1U << CR2_NBYTES_POS);	// NBYTES = 1
	I2C1->CR2 |= CR2_AUTOEND;				// Automatic NACK and STOP

	/* Generate RESTART condition (or START if bus was freed) */
	I2C1->CR2 = CR2_START;

	/* Wait for RXNE flag (or possibly NACKF) */
	while (!(I2C1->ISR & ISR_RXNE)) {
		if (I2C1->ISR & ISR_NACKF) {
			I2C1->ICR |= ICR_NACKCF;
			/* Handle NACK error */
			I2C1->CR2 |= CR2_STOP;
			while (!(I2C1->ISR & ISR_STOPF)) {}
			I2C1->ICR |= ICR_STOPCF;
			return;
		}
	}

	/* Read the received byte from RXDR */
	*data = (uint8_t)I2C1->RXDR;

	/* AUTOEND will handle NACK and STOP. Wait for STOPF */
	while (!(I2C1->ISR & ISR_STOPF)) {}

	/* Clear STOPF flag */
	I2C1->ICR = ICR_STOPCF;
}


void I2C1_BurstRead(char saddr, char maddr, int n, uint8_t *data)
{

	if (n <= 0) return;	// Return if no bytes to read

	// --- Phase 1: Write memory address (maddr) ---
	// Wait until bus not busy
	while (I2C1->ISR & ISR_BUSY) {}

	// Configure CR2 for writing maddr: slave address, 1 byte, AUTOEND=0 (software end for RESTART)
	I2C1->CR2 = 0; 							// Clear CR2 for a fresh configuration
    I2C1->CR2 |= (uint32_t)saddr << 1;      // Set 7-bit slave address
    I2C1->CR2 &= ~CR2_RD_WRN;               // Set transfer direction to WRITE
    I2C1->CR2 |= (1U << CR2_NBYTES_POS);    // Set NBYTES to 1 (for sending maddr)
    I2C1->CR2 &= ~CR2_AUTOEND;              // Disable AUTOEND for this phase to allow RESTART

	// Generate START condition
	I2C1->CR2 |= CR2_START;

	// Wait for TXIS (or NACKF for slave address)
	while (!(I2C1->ISR & ISR_TXIS)) {
		if (I2C1->ISR & ISR_NACKF) {
            I2C1->ICR |= ICR_NACKCF; // Clear NACK flag
            I2C1->CR2 |= CR2_STOP;   // Generate STOP to abort
            while (!(I2C1->ISR & ISR_STOPF)) {}
            I2C1->ICR |= ICR_STOPCF; // Clear STOP flag
            return; // Exit on NACK error
        }
	}

	I2C1->TXDR = maddr; // Send memory address

	// Wait for TC (Transfer Complete for the maddr write)
	// SCL will be stretched low if TC is set and AUTOEND=0
	while (!(I2C1->ISR & ISR_TC)) {
		if (I2C1->ISR & ISR_NACKF) { // Should ideally be caught by TXIS check earlier
			I2C1->ICR |= ICR_NACKCF;
			I2C1->CR2 |= CR2_STOP;
			while (!(I2C1->ISR & ISR_STOPF)) {}
			I2C1->ICR |= ICR_STOPCF;
			return;
		}
	}
	// Note: TC flag is cleared by setting START or STOP bit in I2C_CR2


	// --- Phase 2: Read 'n' bytes of data ---
	// Configure CR2 for read: slave address, 'n' bytes.
	// Use AUTOEND=1 for automatic NACK of last byte and STOP.
	// For >255 byte reads, RELOAD mechanism would be needed.
	if (n > 255) {
		// Error: This simplified function doesn't handle >255 byte reads with RELOAD.
		I2C1->CR2 |= CR2_STOP;
		while (!(I2C1->ISR & ISR_STOPF)) {}
		I2C1->ICR |= ICR_STOPCF;
		return;
	}

	I2C1->CR2 = 0; // Clear CR2 before reconfiguring
    I2C1->CR2 |= (uint32_t)saddr << 1;      // Set 7-bit slave address
    I2C1->CR2 |= CR2_RD_WRN;                // Set transfer direction to READ
    I2C1->CR2 |= ((uint32_t)n << CR2_NBYTES_POS); // Set NBYTES to the number of bytes to read
    I2C1->CR2 |= CR2_AUTOEND;               // Enable AUTOEND: automatically NACKs the last byte
                                            // and generates a STOP condition after NBYTES are received.


	// Generate RESTART condition (this also clears the previous TC flag)
	I2C1->CR2 |= CR2_START;

	for (int i = 0; i < n; i++) {
		// Wait for RXNE (or NACKF if slave NACKed its address for read)
		while (!(I2C1->ISR & ISR_RXNE)) {
			if (I2C1->ISR & ISR_NACKF) {
				I2C1->ICR |= ICR_NACKCF;
				// AUTOEND should handle STOP on NACK.
				return; // Or error code
			}
		}
		data[i] = (uint8_t)I2C1->RXDR;	// Read the received byte into the data buffer
	}

	// With AUTOEND=1, NACK for the last byte and STOP are generated automatically.
	// Wait for the STOPF flag.
	while (!(I2C1->ISR & ISR_STOPF)) {
		// Check for NACKF again, though it implies slave didn't send all data if it occurs here.
		 if (I2C1->ISR & ISR_NACKF) {
			 I2C1->ICR |= ICR_NACKCF;
		 }
	}
	I2C1->ICR |= ICR_STOPCF; // Clear STOPF flag
}


void I2C1_BurstWrite(char saddr, char maddr, int n, uint8_t* data)
{
	// volatile uint32_t tmpreg; // Not strictly needed for F3 flag clearing method

	if (n <= 0) return;

	// Wait until bus not busy
	while (I2C1->ISR & ISR_BUSY) {}

	// Configure CR2 for master write operation
	// Total bytes to send = 1 (for maddr) + n (for data)
	// Ensure this total does not exceed 255 for this simple implementation.
	// For >255 bytes, RELOAD mechanism would be needed.
	uint32_t num_bytes_total = 1 + n;
	if (num_bytes_total > 255) {
		// Error: This simplified function doesn't handle >255 byte writes with RELOAD.
		// Consider generating a STOP and returning an error.
		I2C1->CR2 |= CR2_STOP;
		while (!(I2C1->ISR & ISR_STOPF)) {}
		I2C1->ICR |= ICR_STOPCF;
		return;
	}

	I2C1->CR2 = 0; // Clear CR2 before configuring
	I2C1->CR2 |= (uint32_t)saddr << 1;          // SADD[7:1]
	I2C1->CR2 &= ~CR2_RD_WRN;               // Write operation
	I2C1->CR2 |= (num_bytes_total << CR2_NBYTES_POS); // NBYTES = 1 (maddr) + n (data)
	I2C1->CR2 |= CR2_AUTOEND;               // Automatic STOP after all bytes are sent
	// I2C1->CR2 &= ~CR2_RELOAD;            // Not using reload

	// Generate START condition
	I2C1->CR2 |= CR2_START;

	// Send memory address (maddr)
	// Wait for TXIS (or NACKF)
	while (!(I2C1->ISR & ISR_TXIS)) {
		if (I2C1->ISR & ISR_NACKF) {
			I2C1->ICR |= ICR_NACKCF;
			// AUTOEND should handle STOP on NACK if NACKF is detected by hardware correctly.
			// Or, ensure STOP is generated.
			// while (!(I2C1->ISR & I2C_ISR_STOPF)) {} // Might be handled by AUTOEND
			// I2C1->ICR |= I2C_ICR_STOPCF; // Clear STOPF if it occurs
			return; // Or error code
		}
	}
	I2C1->TXDR = maddr;

	// Send data bytes
	for (int i = 0; i < n; i++) {
		// Wait for TXIS (or NACKF)
		while (!(I2C1->ISR & ISR_TXIS)) {
			if (I2C1->ISR & ISR_NACKF) {
				I2C1->ICR |= ICR_NACKCF;
				// AUTOEND should handle STOP on NACK.
				// while (!(I2C1->ISR & I2C_ISR_STOPF)) {}
				// I2C1->ICR |= I2C_ICR_STOPCF;
				return; // Or error code
			}
		}
		I2C1->TXDR = data[i];
	}

	// With AUTOEND=1, NBYTES will be decremented by hardware, and STOP is generated automatically.
	// Wait for the STOPF flag.
	while (!(I2C1->ISR & ISR_STOPF)) {
		// Check for NACKF again, though it should have been caught earlier
		if (I2C1->ISR & ISR_NACKF) {
			 I2C1->ICR |= ICR_NACKCF; // Clear it, though transfer already failed
			 // STOP should still occur with AUTOEND
		}
	}
	I2C1->ICR |= ICR_STOPCF; // Clear STOP flag
}














