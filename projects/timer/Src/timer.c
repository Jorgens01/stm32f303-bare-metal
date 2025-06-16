/***************************************************************************
 * File name     :  timer.c
 * Description   :  This file provides functions to initialize and control
 *                  the Timer 3 (TIM3) module on an STM32F3 microcontroller.
 *                  It configures TIM3 for a 1-second periodic update event.
 *
 * Author        :  Jere Piirainen
 * Date          :  2025-06-16
 **************************************************************************/
#include "timer.h"
#include "stm32f3xx.h"

void timer3Init(void)
{
	/* Enable clock access to timer3 */
	RCC->APB1ENR |= TIM3EN;

	/* Set prescaler value */
	TIM3->PSC = 800 - 1;		// 8 000 000 / 8 000 = 10 000

	/* Set auto-reload value */
	TIM3->ARR = 10000 - 1;		// 10 000 / 10 000 = 1 Hz

	/* Clear counter */
	TIM3->CNT = 0;

	/* Enable timer */
	TIM3->CR1 = CR1_CEN;
}