/***************************************************************************
 * File name     :      adc.h
 * Description   :      Header file for ADC peripheral configuration and control.
 *                      Provides function prototypes for initializing ADC1,
 *                      starting conversions, and reading ADC values.
 *
 * Author        :      Jere Piirainen
 * Date          :      2025-06-15
 **************************************************************************/

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

/* --- Clock Enable Defines --- */
#define GPIOAEN         (1U << 17)  // Clock enable bit for GPIOA in RCC_AHBENR
#define ADC1EN          (1U << 28)  // Clock enable bit for ADC1/ADC2 in RCC_AHBENR

/* --- ADC Channel and Sequence Defines --- */
#define ADC1_CH1        (1U << 6)   // SQR1_SQ1 bits for Channel 1 (PA1)
#define ADC1_SEQ_LEN    0x00        // L bits in SQR1 = 0000b for 1 conversion in sequence

/* --- ADC Control Register (CR) Bit Defines --- */
#define CR_ADEN         (1U << 0)   // ADC Enable bit
#define CR_ADSTART      (1U << 2)   // ADC Start conversion of regular group
#define CR_ADCAL        (1U << 31)  // ADC Calibration Enable bit

/* --- ADC Interrupt and Status Register (ISR) Bit Defines --- */
#define ISR_ADRDY       (1U << 0)   // ADC Ready flag
#define ISR_EOC         (1U << 2)   // End Of Conversion flag

/* --- ADC Configuration Register (CFGR) Bit Defines --- */
#define CFGR_CONT       (1U << 13)  // Continuous Conversion Mode bit


/**
 * @brief Initializes GPIOA pin 1 (PA1) for analog input and configures the ADC1 module.
 * This includes enabling clocks, voltage regulator, calibration, and basic ADC setup.
 */
void pa1ADCInit(void);

/**
 * @brief Starts continuous ADC conversion on the configured channel.
 * Assumes pa1ADCInit() has been called.
 */
void startConversion(void);

/**
 * @brief Reads the last converted value from the ADC Data Register.
 * This function waits until a conversion is complete.
 * @return The 12-bit digital value from the ADC.
 */
uint32_t adcRead(void);


#endif /* ADC_H_ */
