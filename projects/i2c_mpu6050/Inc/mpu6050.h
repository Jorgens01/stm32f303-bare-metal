/***************************************************************************
 * File name     :  mpu6050.h
 * Description   :  Header file for MPU6050 accelerometer/gyroscope functions.
 *                  Defines device address, register addresses, and declares
 *                  functions for initializing and communicating with the MPU6050.
 *
 * Author        :  Jere Piirainen
 * Date          :  2025-06-18
 **************************************************************************/


#ifndef MPU6050_H_
#define MPU6050_H_

#include <stdint.h>

// --- MPU6050 device address ---
#define MPU6050_DEVICE_ADDR		0x68	// Slave address, AD0 pin low

// --- MPU6050 register addresses ---
#define MPU6050_ACCEL_XOUT_H_REG    0x3B    // X-axis acceleration high byte
#define MPU6050_PWR_MGMT_1_REG      0x6B    // Power management 1 register
#define MPU6050_WHO_AM_I_REG        0x75    // Device identification register
#define MPU6050_ACCEL_CONFIG        0x1C    // Accelerometer configuration register

// --- MPU050 configuration values --- 
#define MPU6050_PWR_MGMT_1_RESET	    0x80	        // Bit 7 device reset
#define MPU6050_PWR_MGMT_1_WAKE_CLKSEL  (0x00 | 0x01)   // Wake up and select PLL with X-axis gyro as clock source
#define MPU6050_ACCEL_FS_4G             (0x01 << 3)     // ±4g full-scale range


/**
 * @brief Initializes the MPU6050 sensor.
 * This function initializes I2C, verifies the device ID, resets the sensor,
 * and configures it with default settings.
 */
void mpu6050_Init(void);

/**
 * @brief Reads a single byte from the specified MPU6050 register.
 * @param reg The register address to read from.
 * @param data Pointer to store the read byte.
 */
void mpu6050_ReadByte(uint8_t reg, uint8_t *data);

/**
 * @brief Writes a single byte to the specified MPU6050 register.
 * @param reg The register address to write to.
 * @param value The byte value to write.
 */
void mpu6050_WriteByte(uint8_t reg, uint8_t value);

/**
 * @brief Reads accelerometer values from the MPU6050.
 * @param accel_x Pointer to store X-axis acceleration.
 * @param accel_y Pointer to store Y-axis acceleration.
 * @param accel_z Pointer to store Z-axis acceleration.
 * @note Values are returned as raw 16-bit integers.
 */
void mpu6050_ReadAccelValues(int16_t *accel_x, int16_t *accel_y, int16_t *accel_z);

#endif /* MPU6050_H__*/