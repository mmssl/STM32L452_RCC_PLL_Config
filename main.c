#define STM32L452xx 1
#include <stdio.h>
#include <stdint.h>
#include "stm32l4xx.h"
#include "Delay_ms.h"

void RCC_PLL_Clock_Config (void)
{
  RCC->CFGR |= (1<<8); // enable HSI
  while(!(RCC->CFGR & (1<<10))){}; // wait for enable	
  RCC->CR &= ~(1<<24); // disable the PLL
  while((RCC->CR & (1<<25)) != (1<<25)){}; // wait until PLLRDY is cleared

  RCC->PLLCFGR &= ~(0x7f<<8) & ~(3<<25) & ~(1<<24) & ~(7<<4) & ~(3<<0); // reset the PLLN, PLLR, PLLM, PLLSRC
  RCC->PLLCFGR |=  (0xb<<8)  |  (2<<0); // set the PLLN and PLLSRC (other paramaters reset state are desired values)
  RCC->CR |= (1<<24); // enable the PLL
  RCC->CFGR |= (1<<24); // enable the PLL output with setting the PLLR pin 1

  RCC->CFGR &= ~(1<<0); // reset the sw bits
  RCC->CFGR |=  (3<<0); // select PLL as system clock
  while(!(RCC->CFGR & RCC_CFGR_SWS_PLL)){}; // wait to be set
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

int main (void)
{
  RCC_PLL_Clock_Config();
  GPIO_Config();
  TIM2Config();

  for(;;)
    {
      GPIOB->ODR ^= (1<<13);
      delay_ms(100);     
    }


}
