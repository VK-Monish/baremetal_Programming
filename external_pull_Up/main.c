/**
 ******************************************************************************
 * @file    external_Pull_Up.c
 * @author  Monish Kumar.k
 * @date    26/12/2024
 * @brief   This program demonstrates reading a button input using an external pull-up resistor
 *          and controlling an LED on an STM32 microcontroller via direct register access.
 *
 *          Functionality:
 *          - Enables the High-Speed External (HSE) clock and configures it as the system clock.
 *          - Enables the clock for GPIO Port A.
 *          - Configures PA4 as an output pin (LED control).
 *          - Configures PA0 as an input pin (button).
 *          - Continuously reads the state of the button connected to PA0.
 *          - Turns the LED (PA4) OFF when the button is pressed (logic LOW due to pull-up).
 *          - Turns the LED ON when the button is released (logic HIGH).
 *
 * @note    Assumes the button is connected to PA0 with an external pull-up resistor.
 *          The LED is active low in this configuration.
 *          The code uses bare-metal programming with direct register manipulation.
 *
 * @usage   Connect a button to PA0 with an external pull-up resistor.
 *          Connect an LED to PA4.
 *          When the button is pressed, the LED turns OFF; when released, the LED turns ON.
 ******************************************************************************
 */


/**
 ******************************************************************************
  Name : Monish Kumar.k
  Date : 26/12/2024
  File : external_Pull_Up
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
		*GPIOA_ODR = *GPIOA_ODR & ~(1<<4);
	}
	else 
	{
		*GPIOA_ODR = *GPIOA_ODR | (1<<4);
	}
}
