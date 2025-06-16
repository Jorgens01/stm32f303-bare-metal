/***************************************************************************
 * File name     :  timer.h
 * Description   :  Header file for Timer3 configuration and control.
 *                  Defines constants for Timer3 settings and declares the
 *                  initialization function.
 *
 * Author        :  Jere Piirainen
 * Date          :  2025-06-16
 **************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_

#define TIM3EN		(1U << 1)   // Clock enable bit for TIM3 in RCC_APB1ENR
#define CR1_CEN		(1U << 0)   // Counter Enable bit in TIMx_CR1
#define SR_UIF		(1U << 0)   // Update Interrupt Flag in TIMx_SR

/**
 * @brief Initializes Timer3 to generate an update event every 1 second.
 * This function configures the prescaler (PSC) and auto-reload register (ARR)
 * to achieve the desired timing.
 */
void timer3Init(void);


#endif /* TIMER_H_ */
