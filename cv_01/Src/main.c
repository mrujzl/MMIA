/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Miroslav Rujzl
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include "stm32f0xx.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
	uint32_t morse = 0xa9ddca80; // S.O.S

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;		// Set clock for GPIOA

	GPIOA->MODER |= GPIO_MODER_MODER5_0;	// GPIO port mode register: pin 5

    /* Loop forever */
	while(1)
	{
		for(uint8_t i = 0; i < sizeof(morse); i++)
		{
			if(morse & (sizeof(morse)-i))
			{
				GPIOA->BSRR = (1<<5);	// LED on
			}
			else
			{
				GPIOA->BRR = (1<<5); 	// LED off
			}

			for(volatile uint32_t i = 0; i < 100000; i++) {}  // Timer
		}
	}
}
