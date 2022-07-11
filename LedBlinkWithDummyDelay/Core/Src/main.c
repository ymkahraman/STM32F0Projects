#include "main.h"

void SystemClock_Config(void);
void GPIO_C_Config(void);
void dummyDelay(void);
int main(void)
{

  SystemClock_Config();
  GPIO_C_Config();
  while (1)
  {
	  GPIOC->ODR |= (1 << 8);
	  GPIOC->ODR |= (1 << 9);
	  dummyDelay();
	  GPIOC->ODR &= ~(1 << 8);
	  GPIOC->ODR &= ~(1 << 9);
	  dummyDelay();
  }
}

// High Speed External Oscillator is used to get better result.
void SystemClock_Config(void)
{
  RCC->CR &= 0x00000083;
  // OFF the HSI oscillator.
  // RCC->CR &= ~(1<<0);
  RCC->CR &= ~RCC_CR_HSION;
  // ON the HSE oscillator. (More accurate.)
  //RCC->CR |= 1 << 16;
  RCC->CR |= RCC_CR_HSEON;
  // Enable Clock security system
  // RCC->CR |= 1 << 19;
  RCC->CR |= RCC_CR_CSSON;
  while((RCC->CR && RCC_CR_HSERDY) != 1)
  {
	  // Waiting untill clock is active
  }
  // 4 ve 12 (PLL Multiplexer bu)
  RCC->CFGR &= 0x00000000;
  RCC->CFGR |= (0b10 << RCC_CFGR_PLLSRC_Pos);
  RCC->CFGR |= (0b1010 << RCC_CFGR_PLLMUL_Pos);
  RCC->CFGR |= (RCC_CFGR_MCO_SYSCLK);
  // PLL is selected as system clock.
  // RCC->CFGR |= (1 << 1);
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  // Check if PLL used as system clock.
  while(RCC->CFGR && RCC_CFGR_SWS_PLL == 0);
  // System clock is now 48MHz.

}
void GPIO_C_Config(void)
{
	// PC9 and PC8 are the LEDs pins.
	// GPIO_C clock source is from AHB.
	RCC->AHBENR |= (1 << 19);
	// Set PC8 and PC9 as General purpose output mode.
	GPIOC->MODER |= (1 << 18);
	GPIOC->MODER &= ~(1 << 19);
	GPIOC->MODER |= (1 << 16);
	GPIOC->MODER &= ~(1 << 17);
	// Push-Pull so it is reset no need to do it.
	GPIOC->OTYPER &= ~(1 << 8);
	GPIOC->OTYPER &= ~(1 << 9);
}
void dummyDelay(void)
{
	for(int i = 0; i < 100000; i++);
}


