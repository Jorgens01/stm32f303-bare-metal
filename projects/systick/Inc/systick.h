/***************************************************************************
 * File name     :  systick.h
 * Description   :  Header file for SysTick timer functions.
 *                  Defines constants for SysTick configuration and declares
 *                  the millisecond delay function.
 *
 * Author        :  Jere Piirainen
 * Date          :  2025-06-16
 **************************************************************************/
#ifndef SYSTICK_H_
#define SYSTICK_H_

/* --- SysTick configuration defines --- */
#define SYSTICK_LOAD_VAL		8000        // For 8MHz system clock, 8000 cycles equals 1 ms

/* --- SysTick control and status register bit defines --- */
#define CSR_ENABLE				(1U << 0)   // Enable SysTick timer
#define CSR_CLKSRC				(1U << 2)   // Select AHB clock as SysTick clock source
#define CSR_COUNTFLAG			(1U << 16)  // Read-only flag: set when counter goes from 1 to 0


/**
 * @brief Provides a blocking delay in milliseconds using the SysTick timer.
 * This function is CPU-blocking; no other tasks can run during the delay.
 * @param delay The desired delay duration in milliseconds.
 * @note This implementation reconfigures SysTick for each call and polls the COUNTFLAG.
 * It assumes the core clock is consistent with SYSTICK_LOAD_VAL.
 */
void systickDelayMs(int delay);

#endif /* SYSTICK_H_ */