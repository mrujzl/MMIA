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

#define GPIOA_PIN04 0x00000010 	// LED1
#define GPIOB_PIN00 0x00000001 	// LED2
#define GPIOC_PIN01	0x00000010	// S1
#define GPIOC_PIN00	0x00000001	// S2
#define LED_TIME_BLINK 	300		   	// 300 ms period for LED1
#define BUTTON_DEBOUNCE 40		   	// 40 ms time for debouncing
#define LED_TIME_SHORT 	100		   	// 100 ms time LED1 on
#define LED_TIME_LONG 	1000		// 1000 ms time LED2 on

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

volatile uint32_t Tick;

void SysTick_Handler(void)
{
	Tick++;
}

void blinker(void)
{
	static uint32_t delay;

	if(Tick > delay + LED_TIME_BLINK)
	{
		GPIOA->ODR ^= GPIOA_PIN04;	// change the LED2 state
		delay = Tick;
	}
}

void buttons(void)
{
	static uint32_t debounce;
	static uint32_t off_time;

	if(Tick > debounce + BUTTON_DEBOUNCE)  //
	{
		static uint32_t old_s1, old_s2;
		uint32_t new_s1 = GPIOC->IDR & GPIOC_PIN01;		// read the S1
		uint32_t new_s2 = GPIOC->IDR & GPIOC_PIN00;		// read the S2

		if (old_s1 && !new_s1) // falling edge S1
		{
			off_time = Tick + LED_TIME_LONG;
			GPIOB->BSRR = GPIOB_PIN00;		// change the LED2 state
		}

		if (old_s2 && !new_s2) // falling edge S2
		{
			off_time = Tick + LED_TIME_SHORT;
			GPIOB->BSRR = GPIOB_PIN00;		// change the LED2 state
		}

		old_s2 = new_s2;
	}

	if(Tick > off_time)
	{
		GPIOB->BRR = GPIOB_PIN00;	// change the LED2 state
	}
}

void EXTI0_1_IRQHandler(void)
{
	if(EXTI->PR & EXTI_PR_PR0) // check line 0 has triggered the IT
	{
		EXTI->PR |= EXTI_PR_PR0; // clear the pending bit
		GPIOB->ODR ^=  GPIOB_PIN00;	// change the LED2 state
	}
}

int main(void)
{
	SysTick_Config(8000); // period 1 ms

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN; // enable
	GPIOA->MODER |= GPIO_MODER_MODER4_0; 	// LED1 = PA4, output
	GPIOB->MODER |= GPIO_MODER_MODER0_0; 	// LED2 = PB0, output
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR0_0; 	// S2 = PC0, pullup
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR1_0; 	// S1 = PC1, pullup

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; 	// enable SYSCFGEN

/*	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PC; // select PC0 for EXTI0
	EXTI->IMR |= EXTI_IMR_MR0; 				// mask
	EXTI->FTSR |= EXTI_FTSR_TR0; 			// trigger on falling edge
	NVIC_EnableIRQ(EXTI0_1_IRQn);			// enable EXTI0_1
*/
    /* Loop forever */
	while(1)
	{
		blinker();
		buttons();
	}
}
