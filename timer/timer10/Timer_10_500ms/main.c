/**
 ******************************************************************************
  Name : Monish Kumar.k
  Date : 13/01/2025
  File : Timer_10_500ms
 ******************************************************************************/
#include <arm.h>

void rcc_Config(void);
void choose_Port_C(void);
void gpio_Moder(void);
void led_Blink_C13(void);
void timer(void);
void delay(int delay);

int main(void)
{
	rcc_Config();
	choose_Port_C();
	gpio_Moder();
	while(1)
	{
		led_Blink_C13();
	}
}

void rcc_Config()
{
	RCC->CR = RCC->CR & (~0x00010000); /*HES is disable */

	RCC->CR = RCC->CR | (1<<16); /*HES is Enable*/

	//clock configuration

	RCC->CFGR = RCC->CFGR & (~0x00000003); /* disable the sw0 and sw1*/

	RCC->CFGR = RCC->CFGR | (1<<0);

}

void choose_Port_C()
{
	RCC->AHB1ENR = RCC->AHB1ENR | (1<<2);
	RCC->APB2ENR = RCC->APB2ENR | (1<<17);
}

void gpio_Moder()
{
	GPIOC->MODER = GPIOC->MODER | (1<<26);
}

void timer()
{
	TIM10->CR1 |= (1<<1);   //UDIS: Update disable

	TIM10->CR1 &= ~(1<<2);  //URS: Update request source

	TIM10->CR1 |= (1<<7);   //Auto-reload preload enable

	TIM10->CNT = 0;                   //counter
}

void delay(int delay)
{
	TIM10->PSC = 999;                  //prescaler

	TIM10->ARR = delay;               //auto-reload register

	TIM10->CR1 |= (1<<0);   //CEN: Counter enable

	while(!(TIM10->SR & (1<<0)));

	TIM10->SR &= ~(1<<0);
}

void led_Blink_C13()
{
	GPIOC->ODR = GPIOC->ODR & ~(1<<13); // LED on
	delay(12500);
	GPIOC->ODR = GPIOC->ODR | (1<<13); // LED off
	delay(12500);
}
