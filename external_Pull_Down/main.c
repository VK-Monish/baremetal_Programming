/**
 ******************************************************************************
 * @file    external_Pull_Down.c
 * @author  Monish Kumar.k
 * @date    26/12/2024
 * @brief   This program configures GPIO Port A pins on an STM32 microcontroller 
 *          to demonstrate reading a button input with an external pull-down resistor 
 *          and controlling an LED accordingly.
 *
 *          Functionality:
 *          - Enables High-Speed External (HSE) clock and selects it as the system clock.
 *          - Enables GPIO Port A clock.
 *          - Configures PA4 as an output pin (for LED).
 *          - Configures PA0 as an input pin (button).
 *          - Reads the input state of PA0 continuously.
 *          - Turns ON the LED connected to PA4 when the button (PA0) is pressed (logic HIGH).
 *          - Turns OFF the LED otherwise.
 *
 * @note    Assumes an external pull-down resistor is connected to the button input pin (PA0).
 *          The code uses direct register manipulation for bare-metal programming.
 *
 * @usage   Connect a button to PA0 with an external pull-down resistor.
 *          Connect an LED to PA4.
 *          When the button is pressed, the LED will turn ON; otherwise, it will be OFF.
 ******************************************************************************
 */

/**
 ******************************************************************************
  Name : Monish Kumar.k
  Date : 26/12/2024
  File : external_Pull_Down
 ******************************************************************************/
volatile unsigned int *RCC_CR      = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_CFGR    = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;

volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000;
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014;
volatile unsigned int *GPIOA_IDR   = (volatile unsigned int *)0x40020010;

void rcc_Config(void);
void choose_Port_A(void);
void gpio_Moder(void);
void button_Config(void);
void delay_Ms(int ms);

int main(void)
{
	rcc_Config();
	choose_Port_A();
	gpio_Moder();
	while(1)
	{
		button_Config();
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

void choose_Port_A()
{
	*RCC_AHB1ENR = *RCC_AHB1ENR | (1<<0);
}

void gpio_Moder()
{
	*GPIOA_MODER = *GPIOA_MODER | (1<<8);
	*GPIOA_MODER = *GPIOA_MODER & (~0x00000003);
}

void delay_Ms(int ms)
{
	for(int i=0; i<ms*4000; i++)
	{
		__asm("NOP");
	}
}

void button_Config()
{
	if((*GPIOA_IDR & (0X00000001)) == 1)
	{
		*GPIOA_ODR = *GPIOA_ODR | (1<<4);
	}
	else
	{
		*GPIOA_ODR = *GPIOA_ODR & ~(1<<4);
	}
}
