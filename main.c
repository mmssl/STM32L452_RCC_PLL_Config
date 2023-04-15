#define STM32L452xx 1
#include <stdio.h>
#include <stdint.h>
#include "stm32l4xx.h"
#include "Delay_ms.h"

void PLLConfig (void);
void GPIO_Config (void);


void PLLConfig (void)
{
  RCC->CR &= ~(1u<<0);
  RCC->CR |= (1<<8); // enable HSI
  while(!(RCC->CR & (1<<10))){} // wait for enable
  
  RCC->AHB1ENR |= (1<<8); // Enable FLASH clock
  RCC->APB1ENR1 |= (1<<28); // Enable Power clock
				
  PWR->CR1 |= (1<<9); // Set VDD range to 1
  FLASH->ACR |= (4<<0); // 4 Latency
  FLASH->ACR |= (1<<8) | (1<<10); // Prefecth enable, Data cache enable
		
  RCC->CR &= ~(1u<<24); // disable the PLL
  while((RCC->CR & RCC_CR_PLLRDY)){} // wait PLL is unlocked
		
  RCC->PLLCFGR &= ~((0x7f<<8) | ~(1<<24) | ~(7<<4) | ~(3<<0)); // reset the PLLN, PLLR, PLLM, PLLSRC
  RCC->PLLCFGR |=  (10<<8)  |  (2<<0); // set the PLLN and PLLSRC (other paramaters reset state are desired values)
 
  RCC->CFGR &= ~((0xf<<4) | ~(0x7<<8)); // Set AHB /1 and APB1 /1 	
  RCC->CR   |= (1<<24); // enable the PLL
  RCC->PLLCFGR |= (1<<24); // enable the PLL output with setting the PLLR pin 1
  while(!(RCC->CR & RCC_CR_PLLRDY)){} // wait PLLRDY is locked		

  RCC->CFGR |=  RCC_CFGR_SW_PLL; // select PLL as system clock
  while((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL){}  // wait system clock to switch pll
}


void GPIO_Config (void)
{
  RCC->AHB2ENR |= (1<<2);
  RCC->AHB2ENR |= (1<<1);
  //LED PB13
  GPIOB->MODER &= ~(3u<<26);
  GPIOB->MODER |=  (1<<26);
  // BUTTON PC13
  GPIOC->MODER &= ~(3u<<26);

  // 3. Configure the OUTPUT MODE
  GPIOB->OTYPER &= ~GPIO_OTYPER_ODR_13;
  GPIOB->OSPEEDR = (1<<27);
}

int main (void)
{
  PLLConfig();
  GPIO_Config();
  TIM2Config();

  for(;;)
    {
      GPIOB->ODR ^= (1<<13);
      delay_ms(1000); // 1s delay 
    }
}
