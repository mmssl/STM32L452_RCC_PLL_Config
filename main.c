#define STM32L452xx 1
#include <stdio.h>
#include <stdint.h>
#include "stm32l4xx.h"
#include "Delay_ms.h"

void GPIO_Config (void);
void ConfigInterrupts (void);

volatile int flag = 0;
volatile int count = 0;

int main (void)
{
  SystemInit();
  SystemCoreClockUpdate();
  GPIO_Config();
  //ConfigInterrupts();
  TIM2Config();


  while (1)
  {
    delay_ms(1000);
    GPIOB->ODR ^= (1<<13);   
  }
return 0;
}

void GPIO_Config (void)
{
	RCC->AHB2ENR |= (1<<2);
	RCC->AHB2ENR |= (1<<1);
	/*LED PB13*/
	GPIOB->MODER &= ~(3<<26);
	GPIOB->MODER |=  (1<<26);
	// BUTTON PC13
	GPIOC->MODER &= ~(3<<26);

	// 3. Configure the OUTPUT MODE
	GPIOB->OTYPER &= ~GPIO_OTYPER_ODR_13;
	GPIOB->OSPEEDR = (1<<27);
}

void ConfigInterrupts (void)
{
  /****>>>>>>> To be Completed  <<<<<<<<*****
1. Enable the AFIO CLOCK bit in RCC register
2. Configure the EXTI configuration Regiter in the AFIO
3. Disable the EXTI Mask using Interrupt Mask Register (IMR)
4. Configure the Rising Edge / Falling Edge Trigger
5. Set the Interrupt Priority
6. Enable the interrupt
********************/

  RCC->APB2ENR |= (1<<0);


  SYSCFG->EXTICR[3] &= ~(3<<4);
  SYSCFG->EXTICR[3] |= (2<<4);

  EXTI->IMR1 |= (1<<13);
  EXTI->RTSR1 |= (1<<13);
  EXTI->FTSR1 &= ~(1<<13);

  NVIC_SetPriority(EXTI15_10_IRQn, 1); //set the Priority
  NVIC_EnableIRQ(EXTI15_10_IRQn);      // Enable interrupt

}

void EXTI15_10_IRQHandler(void)
{
  /****>>>>>>> To be Completed <<<<<<<<*****
	1. Check the Pin, which trigerred the Interrupt
	2. Clear the Interrupt Pending Bit
	3. Update flag variable
	********************/

  if(EXTI->PR1 & (1<<13))
  {
    EXTI->PR1 |= (1<<13);
    flag = 1;
  }
}
