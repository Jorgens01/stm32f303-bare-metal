#include "stm32f3xx.h"
#include "uart.h"
#include "mpu6050.h"
#include "systick.h"
#include "printf.h"

/* Variables to store processed accelerometer values */
int16_t x, y, z;
float xg, yg, zg;

int main(void)
{
	// We have to enable FPU
	#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
		SCB->CPACR |= ((3U << (10*2)) | (3UL << (11*2)));
	#endif 


    uart3_tx_rx_init(); // Initialize UART3 (required for _putchar to work)


	mpu6050_Init();
    while(1) {
    	/* Read 16-bit raw accelerometer values into x, y, z */
    	mpu6050_ReadAccelValues(&x, &y, &z);

    	/* COnvert raw values */
    	xg = (float)x / 8192.0f;
    	yg = (float)y / 8192.0f;
    	zg = (float)z / 8192.0f;

		printf("xg = %f yg = %f, zg = %f\n\r", xg, yg, zg);

    	/* Add small delay here */
    	systickDelayMs(100);

    }
}
