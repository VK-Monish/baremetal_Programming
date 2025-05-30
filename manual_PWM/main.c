/**
 ******************************************************************************
 * @file    Manual_PWM.c
 * @author  Monish Kumar.k
 * @date    16/01/2025
 * @brief   Manual PWM implementation using TIM10 timer on STM32 to vary LED brightness.
 *
 * @details
 * This program demonstrates a software-based PWM (Pulse Width Modulation) technique
 * by manually controlling the ON and OFF durations of an LED connected to PC15.
 * The TIM10 timer is configured for delay generation, and duty cycle is varied in a
 * loop to simulate PWM behavior, producing a fading LED effect.
 *
 * Functionalities:
 *  - RCC clock configuration to enable HSE oscillator
 *  - Enabling GPIOC and TIM10 peripheral clocks
 *  - Configuring PC15 as output pin for LED control
 *  - Manual PWM implemented by varying delay durations for ON and OFF states
 *  - LED brightness varies by changing duty cycle in increments and decrements
 *
 * Note:
 *  - This is a simple, blocking software PWM; timer interrupts or hardware PWM
 *    are not used.
 *  - Delay and brightness depend on timer settings and CPU clock frequency.
 ******************************************************************************
 */

/**
 ******************************************************************************
  Name : Monish Kumar.k
  Date : 16/01/2025
  File : Manual_PWM
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
	GPIOC->MODER = GPIOC->MODER | (1<<28);
	GPIOC->MODER = GPIOC->MODER | (1<<30);
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
	int i=25,j=490;
	for(i=25; i<=475; i=i+25)
	{
		GPIOC->ODR = GPIOC->ODR | (1<<15); // LED on
		delay(i);
		GPIOC->ODR = GPIOC->ODR & ~(1<<15); // LED off
		delay(j);
		j=j-25;
	}
	for(j=25; j<=475; j=j+25)
	{
		GPIOC->ODR = GPIOC->ODR | (1<<15); // LED on
		delay(i);
		GPIOC->ODR = GPIOC->ODR & ~(1<<15); // LED off
		delay(j);
		i=i-25;
	}
}
