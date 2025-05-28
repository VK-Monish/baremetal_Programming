/**
 ******************************************************************************
 * @file    internal_PullDown_Pupdr.c
 * @author  Monish Kumar.k
 * @date    28/12/2024
 * @brief   Demonstrates configuring GPIO pin with internal pull-down resistor on STM32.
 *
 *          Description:
 *          - Enables the High-Speed External (HSE) clock and selects it as system clock.
 *          - Enables clock for GPIO Port A.
 *          - Configures PA0 as input with an internal pull-down resistor enabled.
 *          - Configures PA1 as output (connected to an LED or any indicator).
 *          - Continuously reads input from PA0.
 *          - If PA0 is HIGH, turns ON the LED connected to PA1.
 *          - If PA0 is LOW, turns OFF the LED connected to PA1.
 *
 * @note    The internal pull-down resistor ensures that the PA0 input is LOW by default
 *          when not driven externally HIGH.
 *          Use a switch or sensor to drive PA0 HIGH to turn ON the LED on PA1.
 *
 *          This example uses direct register manipulation (bare-metal programming).
 *
 * @usage   Connect an input device (e.g., button) to PA0.
 *          Connect an LED with resistor to PA1.
 ******************************************************************************
 */

/**
 ******************************************************************************
  Name : Monish Kumar.k
  Date : 28/12/2024
  File : internal_PullDown_Pupdr
 ******************************************************************************/
volatile unsigned int *RCC_CR        = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_CFGR      = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR   = (volatile unsigned int *)0x40023830;

volatile unsigned int *GPIOA_MODER   = (volatile unsigned int *)0x40020000;
volatile unsigned int *GPIOA_ODR     = (volatile unsigned int *)0x40020014;
volatile unsigned int *GPIOA_IDR     = (volatile unsigned int *)0x40020010;
volatile unsigned int *GPIOA_PUPDR   = (volatile unsigned int *)0x4002000C;

void rcc_Config(void);
void choose_Port(void);
void gpio_Moder(void);
void led(void);

int main()
{
	rcc_Config();
	choose_Port();
	gpio_Moder();
	while(1)
	{
		led();
	}
}

void rcc_Config()
{
	*RCC_CR = *RCC_CR & (~0x00010000); /*HES is diable */

	*RCC_CR = *RCC_CR | (1<<16); /*HES is Enable*/

	//clock configuration

	*RCC_CFGR = *RCC_CFGR & (~0x00000003); /* disable the sw0 and sw1*/

	*RCC_CFGR = *RCC_CFGR | (1<<0);
}

void choose_Port()
{
	*RCC_AHB1ENR = *RCC_AHB1ENR | (1<<0);
}

void gpio_Moder()
{
	*GPIOA_MODER = *GPIOA_MODER & (~1<<1);
	*GPIOA_MODER = *GPIOA_MODER & (~1<<0);
	*GPIOA_MODER = *GPIOA_MODER | (1<<2);
	//internal pull_Down
	*GPIOA_PUPDR = *GPIOA_PUPDR | (1<<1);
}

void led()
{
	if(*GPIOA_IDR & (0x00000001))
	{
		*GPIOA_ODR = *GPIOA_ODR | (1<<1);
	}
	else if(*GPIOA_IDR & ~(0x00000001))
	{
		*GPIOA_ODR = *GPIOA_ODR & (~1<<1);
	}
}
