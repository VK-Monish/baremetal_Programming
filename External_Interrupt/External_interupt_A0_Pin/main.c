/**
 ******************************************************************************
 * Project Title : External Interrupt with Rising Edge on GPIOA Pin 0
 * Author        : Monish Kumar K.
 * Date          : 30/12/2024
 * File          : external_Interrupt_A0_Pin.c
 * Description   :
 *     This project demonstrates how to configure an external interrupt on 
 *     STM32F4 using **rising edge trigger** on pin **PA0** (e.g., button release).
 *     When the interrupt is triggered, the interrupt handler blinks an LED 
 *     connected to **PA5** five times.
 *
 * Hardware Setup:
 *     - PA0: Input with pull-down or button tied to logic low by default.
 *     - PA5: Output connected to an LED.
 *     - Rising edge interrupt is triggered by a transition from 0 to 1 on PA0.
 *
 * Functional Summary:
 *     - HSE (High-Speed External) clock is enabled and set as system clock.
 *     - GPIOA and SYSCFG clocks are enabled.
 *     - PA5 is configured as output, PA0 as input with pull-down.
 *     - EXTI line 0 is mapped to PA0.
 *     - Interrupts are enabled for rising edge on EXTI line 0.
 *     - ISR (EXTI0_IRQHandler) blinks the LED 5 times on interrupt.
 *
 * Register Usage:
 *     - RCC_CR, RCC_CFGR       : Clock control (HSE and system clock).
 *     - RCC_AHB1ENR            : Enables GPIOA peripheral.
 *     - RCC_APB2ENR            : Enables SYSCFG (needed for EXTI configuration).
 *     - GPIOA_MODER            : Sets PA0 as input, PA5 as output.
 *     - GPIOA_PUPDR            : Pull-down on PA0 to detect rising edge clearly.
 *     - SYSCFG_EXTICR1         : Maps EXTI0 line to PA0.
 *     - EXTI_IMR, EXTI_RTSR    : Interrupt mask and rising edge trigger for EXTI0.
 *     - EXTI_PR                : Clears the pending interrupt.
 *     - NVIC_ISER0             : Enables EXTI0 interrupt in NVIC.
 *
 * Notes:
 *     - All configuration is done at the register level (no HAL or CMSIS).
 *     - The project is ideal for learning STM32 interrupt handling.
 *     - Rising edge is typically used for detecting button release.
 *
 ******************************************************************************
 */

/**
 ******************************************************************************
  Name : Monish Kumar.k
  Date : 30/12/2024
  File : external_Interrupt_A0_Pin
 ******************************************************************************/
volatile unsigned int *RCC_CR         = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_CFGR       = (volatile unsigned int *)0x40023808;

volatile unsigned int *RCC_AHB1ENR    = (volatile unsigned int *)0x40023830;
volatile unsigned int *RCC_APB2ENR    = (volatile unsigned int *)0x40023844;

volatile unsigned int *GPIOA_MODER    = (volatile unsigned int *)0x40020000;
volatile unsigned int *GPIOA_ODR      = (volatile unsigned int *)0x40020014;
volatile unsigned int *GPIOA_PUPDR    = (volatile unsigned int *)0x4002000C;

//interrupt register......
volatile unsigned int *EXTI_IMR       = (volatile unsigned int *)0x40013C00;
volatile unsigned int *EXTI_RTSR      = (volatile unsigned int *)0x40013C08;
volatile unsigned int *EXTI_PR        = (volatile unsigned int *)0x40013C14;

volatile unsigned int *SYSCFG_EXTICR1 = (volatile unsigned int *)0x40013808;
volatile unsigned int *NVIC_ISER0     = (volatile unsigned int *)0xE000E100;

void rcc_Config(void);
void choose_Port(void);
void gpio_Moder(void);
void exti_Config(void);
void delay(int ms);
void EXTI0_IRQHandler(void);

int main()
{
	rcc_Config();
	choose_Port();
	gpio_Moder();
	exti_Config();
	while(1)
	{
		*GPIOA_ODR |= (1<<5);
	}
}

void rcc_Config()
{

	*RCC_CR = *RCC_CR & (~0x00010000); /*HES is diable */

	*RCC_CR = *RCC_CR | (1<<16); /*HES is Enable*/

	while(!(*RCC_CR & (1<<17)));

	//clock configuration

	*RCC_CFGR = *RCC_CFGR & (~0x00000003); /* disable the sw0 and sw1*/

	*RCC_CFGR = *RCC_CFGR | (1<<0);

	while(!(*RCC_CFGR & (1<<4)));
}

void choose_Port()
{
	*RCC_AHB1ENR  |=  (1<<0);
	*RCC_APB2ENR  |=  (1<<14);
}

void gpio_Moder()
{
	*GPIOA_MODER = (*GPIOA_MODER & ~(0x3 << 0)) | (1<<10);
	*GPIOA_PUPDR = (*GPIOA_PUPDR & ~(0x3 << 0)) | (1 << 0);
}

void exti_Config()
{
	*SYSCFG_EXTICR1 = (*SYSCFG_EXTICR1 & ~(0xF << 0)) | (0<<0);

	*EXTI_IMR   |= (1<<0);

	*EXTI_RTSR  |= (1<<0);

	*NVIC_ISER0 |= (1<<6);
}

void delay(int ms)
{
	for(int i=0;i<ms*4000;i++)
	{
		__asm("NOP");
	}
}
void EXTI0_IRQHandler()
{
	if(*EXTI_PR & (1<<0))
	{
		for(int i=0; i<5; i++)
		{
			*GPIOA_ODR &= ~(1<<5);
			delay(100);
			*GPIOA_ODR |= (1<<5);
			delay(100);
		}
	   *EXTI_PR |= (1<<0);
	}
}
