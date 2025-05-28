/**
 ******************************************************************************
 * @file    default_C13.c
 * @author  Monish Kumar.k
 * @date    23/12/2024
 * @brief   This program configures GPIO Port C, pin 13 (PC13) on an STM32 
 *          microcontroller to blink an LED connected to that pin.
 * 
 *          The program performs the following:
 *          1. Enables the High-Speed External (HSE) clock.
 *          2. Selects HSE as the system clock source.
 *          3. Enables the clock for GPIO Port C.
 *          4. Configures PC13 as a digital output pin.
 *          5. Toggles the LED connected to PC13 ON and OFF with a 1-second delay.
 * 
 *          Delay is implemented via a simple busy-wait loop.
 * 
 * @note    The code directly manipulates hardware registers using memory-mapped
 *          addresses for bare-metal STM32 programming.
 * 
 * @usage   Compile and flash this code to an STM32 MCU with an LED connected 
 *          to PC13. The LED will blink ON and OFF every second.
 ******************************************************************************
 */

/**
 ******************************************************************************
  Name : Monish Kumar.k
  Date : 23/12/2024
  File : default_C13
 ******************************************************************************/
volatile unsigned int *RCC_CR      = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_CFGR    = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;

volatile unsigned int *GPIOC_MODER = (volatile unsigned int *)0x40020800;
volatile unsigned int *GPIOC_ODR   = (volatile unsigned int *)0x40020814;

void rcc_Config(void);
void choose_Port_C(void);
void gpio_Moder(void);
void led_Blink_C13(void);
void delay_Ms(int ms);

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
	*RCC_CR = *RCC_CR & (~0x00010000); /*HES is diable */

	*RCC_CR = *RCC_CR | (1<<16); /*HES is Enable*/

	while(!(*RCC_CR & (1<<17))==0);

	//clock configuration

	*RCC_CFGR = *RCC_CFGR & (~0x00000003); /* disable the sw0 and sw1*/

	*RCC_CFGR = *RCC_CFGR | (1<<0);

	while(!(*RCC_CFGR & (1<<4))==4);

}

void choose_Port_C()
{
	*RCC_AHB1ENR = *RCC_AHB1ENR | (1<<2);
}

void gpio_Moder()
{
	*GPIOC_MODER = *GPIOC_MODER | (1<<26);
}

void delay_Ms(int ms)
{
	for(int i=0; i<ms*4000; i++)
	{
		__asm("NOP");
	}
}

void led_Blink_C13()
{
	*GPIOC_ODR = *GPIOC_ODR & ~(1<<13); // LED on
	delay_Ms(1000);
	*GPIOC_ODR = *GPIOC_ODR | (1<<13); // LED off
	delay_Ms(1000);
}
