/***************************************************************************
 * File name     :      systick.c
 * Description   :      This file implements a blocking delay function using
 *                      the ARM Cortex-M SysTick timer. It provides a millisecond
 *                      delay utility based on the system clock.
 *
 * Author        :      Jere Piirainen
 * Date          :      2025-06-16
 **************************************************************************/
#include "systick.h"
#include "stm32f3xx.h"

void systickDelayMs(int delay)
{
	/* Configure SysTick timer for 1 ms counting */
	/* Reload with number of clock cycles per millisecond */
	SysTick->LOAD = SYSTICK_LOAD_VAL;

	/* Clear SysTick current value register */
	SysTick->VAL = 0;

	/* Enable SysTick and select internal clock source */
	SysTick->CTRL = (CSR_ENABLE | CSR_CLKSRC);

	for (int i = 0; i < delay; i++) {
		/* Wait until the countflag is set */
		while ((SysTick->CTRL & CSR_COUNTFLAG) == 0) {}
	}
    /* Disable SysTick timer after delay completion */
	SysTick->CTRL = 0;
}