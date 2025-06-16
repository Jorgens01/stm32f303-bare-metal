/***************************************************************************
 * File name     :		adc.c
 * Description   : 		This file provides functions to initialize and control
 * 					  	the ADC1 module on an STM32F3 microcontroller.
 * 						It configures PA1 as an analog input, performs ADC
 * 						calibration, and enables continuous conversion to read
 * 						analog sensor data.
 *
 * Author        :    	Jere Piirainen
 * Date          :    	2025-06-15
 **************************************************************************/

#include "adc.h"
#include "stm32f3xx.h"


void pa1ADCInit(void)
{
	/* --- Configure ADC GPIO pin --- */
	/* Enable clock access to GPIOA */
	RCC->AHBENR |= GPIOAEN;

	/* Set mode of PA1 to analog mode (0b11) */
	GPIOA->MODER |= (1U << 2);
	GPIOA->MODER |= (1U << 3);


	/* --- Configure the ADC1 module --- */
	/* Enable clock access to ADC */
	RCC->AHBENR |= ADC1EN;

	/* Configure ADC common control register (CCR) for clock source */
	ADC1_2_COMMON->CCR &= ~((1U << 17) | (1U << 16));
	ADC1_2_COMMON->CCR |= (0x1U << 16);		// CKMODE to HCLK / 1 (synchronous)


	/* Step 1: Enable voltage regulator ADVREGEN[1:0], bits [29:28] to 01 */
	ADC1->CR &= ~(1U << 29);
	ADC1->CR |= (1U << 28);

	/* Small delay to allow regulator to stabilaze */
	for (volatile int i = 0; i < 1000; i++);


	/* Step 2: Clear ADEN bit to ensure ADC is disabled before calibration */
	ADC1->CR &= ~CR_ADEN; 


	/* Step 3: Calibrate ADC */
	ADC1->CR |= CR_ADCAL;

	/* Wait for ADCAL bit to be cleared */
	while (ADC1->CR & CR_ADCAL) {}


	/* Step 4: Enable ADC module (after calibration) */
	ADC1->CR |= CR_ADEN;

	/* Wait for ADC to be ready */
	while (!(ADC1->ISR & ISR_ADRDY)) {}


	/* Step 5: configure conversion sequence and length */
	/* Conversion sequence start */
	ADC1->SQR1 = ADC1_CH1;

	/* Conversion sequence length */
	ADC1->SQR1 |= ADC1_SEQ_LEN;
}


void startConversion(void)
{
	/* Enable continuous conversion */
	ADC1->CFGR |= CFGR_CONT;

	/* Start ADC conversion */
	ADC1->CR |= CR_ADSTART;
}


uint32_t adcRead(void)
{
	/* Wait for conversion to be complete */
	while (!(ADC1->ISR & ISR_EOC)) {

	}

	/* Read converted result */
	return (ADC1->DR);
}



