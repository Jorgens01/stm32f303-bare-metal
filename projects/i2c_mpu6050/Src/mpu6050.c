/***************************************************************************
 * File name     :  mpu6050.c
 * Description   :  Implementation of MPU6050 accelerometer/gyroscope functions.
 *                  Provides initialization, register read/write, and data
 *                  acquisition functions for the MPU6050 sensor.
 *
 * Author        :  Jere Piirainen
 * Date          :  2025-06-18
 **************************************************************************/

#include "mpu6050.h"
#include "i2c.h"

// Global buffer for raw accelerometer data 
static uint8_t accel_raw_data[6];

void mpu6050_ReadByte(uint8_t reg, uint8_t *data)
{
	// Use I2C1_ByteRead to read a single byte from the specified register
	I2C1_ByteRead(MPU6050_DEVICE_ADDR, reg, data);
}

void mpu6050_WriteByte(uint8_t reg, uint8_t value)
{
	// Create temporary buffer for single byte to write 
	uint8_t data_to_write[1];	// Using char to match I2C1_BurstWrite parameter type
	data_to_write[0] = value;

	// Write the single byte to the specified register
	I2C1_BurstWrite(MPU6050_DEVICE_ADDR, reg, 1, data_to_write);
}
void mpu6050_ReadAccelValues(int16_t *accel_x, int16_t *accel_y, int16_t *accel_z)
{
	// Read 6 bytes starting from ACCEL_XOUT_H register (0x3B)
	I2C1_BurstRead(MPU6050_DEVICE_ADDR, MPU6050_ACCEL_XOUT_H_REG, 6, accel_raw_data);

	// Combine high and low bytes to form 16-bit signed integers 
	*accel_x = (int16_t)((accel_raw_data[0] << 8) | accel_raw_data[1]);
	*accel_y = (int16_t)((accel_raw_data[2] << 8) | accel_raw_data[3]);
	*accel_z = (int16_t)((accel_raw_data[4] << 8) | accel_raw_data[5]);
}

void mpu6050_Init(void)
{
	uint8_t who_am_i_val;

	// Enable I2C peripheral 
	I2C1_Init();

	// Check WHO_AM_I register 
	mpu6050_ReadByte(MPU6050_WHO_AM_I_REG, &who_am_i_val);

	// Reset MPU-6050 
	mpu6050_WriteByte(MPU6050_PWR_MGMT_1_REG, MPU6050_PWR_MGMT_1_RESET);
	for (volatile int i = 0; i < 10000; i++) {}

	// Wake up MPU-6050 and select clock source 
	mpu6050_WriteByte(MPU6050_PWR_MGMT_1_REG, MPU6050_PWR_MGMT_1_WAKE_CLKSEL);

	// Set accelerometer full-scale range 
	mpu6050_WriteByte(MPU6050_ACCEL_CONFIG, MPU6050_ACCEL_FS_4G);
}