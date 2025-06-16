#include <stdint.h>
#include "stm32f3xx.h"
#include "uart.h"
#include "adc.h"

uint32_t sensor_value;

int main(void)
{
	uart3_tx_rx_init();
	pa1ADCInit();
	startConversion();

    uart3_puts("ADC sensor monitor\r\n");

    while(1) {
    	sensor_value = adcRead();       // Read raw ADC value
    	uart3_puts("Sensor value: ");
        uart3_put_int(sensor_value);
        uart3_puts(" (raw ADC)\r\n");


        for (volatile int i = 0; i < 1000000; i++);
    }
}


