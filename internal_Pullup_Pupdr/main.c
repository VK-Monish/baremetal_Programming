/**
 ******************************************************************************
 * @file    internal_Pullup_Pupdr.c
 * @author  Monish Kumar.k
 * @date    28/12/2024
 * @brief   Demonstrates configuring an STM32 GPIO pin with an internal pull-up resistor
 *          and reading a button input to control an LED using direct register manipulation.
 *
 *          Functionality:
 *          - Enables the High-Speed External (HSE) clock and configures it as the system clock.
 *          - Enables clock for GPIO Port A.
 *          - Configures PA0 as an input with an internal pull-up resistor enabled.
 *          - Configures PA1 as an output to drive an LED.
 *          - Continuously reads the input state of PA0 (button).
 *          - Turns the LED ON when the button is pressed (logic LOW due to pull-up).
 *          - Turns the LED OFF when the button is released (logic HIGH).
 *
 * @note    The internal pull-up resistor is enabled via the GPIOx_PUPDR register.
 *          The LED output is connected to PA1.
 *          The button should be connected to PA0, with no external pull-up resistor needed.
 *          The code directly manipulates hardware registers (bare-metal approach).
 *
 * @usage   Connect a button to PA0 (no external pull-up needed).
 *          Connect an LED to PA1.
 *          When the button is pressed, the LED turns ON; when released, the LED turns OFF.
 ******************************************************************************
 */

/**
 ******************************************************************************
  Name : Monish Kumar.k
  Date : 28/12/2024
  File : internal_Pullup_Pupdr
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
	*GPIOA_MODER = *GPIOA_MODER & (~1<<0);
	*GPIOA_MODER = *GPIOA_MODER & (~1<<1);
	*GPIOA_MODER = *GPIOA_MODER | (1<<2);
	//internal pull_up
	*GPIOA_PUPDR = *GPIOA_PUPDR | (1<<0);
}

void led()
{
	if(*GPIOA_IDR & (0x00000001))
	{
		*GPIOA_ODR = *GPIOA_ODR & (~1<<1);
	}
	else if(*GPIOA_IDR & ~(0x00000001))
	{
		*GPIOA_ODR = *GPIOA_ODR | (1<<1);
	}
}
