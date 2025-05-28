/**
 ******************************************************************************
 * @file    interface_Ir_Sensor.c
 * @author  Monish Kumar.k
 * @date    29/12/2024
 * @brief   Interface an IR sensor with STM32 using GPIO and internal pull-up resistor.
 *
 *          Description:
 *          - Enables the High-Speed External (HSE) clock and selects it as system clock.
 *          - Enables clock for GPIO Port A.
 *          - Configures PA0 as input with internal pull-up resistor enabled for IR sensor input.
 *          - Configures PA1 as output to drive an LED or indicator.
 *          - Continuously reads the input from IR sensor on PA0.
 *          - Turns ON the LED (PA1) when IR sensor output is LOW (object detected).
 *          - Turns OFF the LED when IR sensor output is HIGH (no object).
 *          - Adds a delay of ~1 second when LED is ON to debounce or indicate presence.
 *
 * @note    IR sensor output is connected to PA0 (input).
 *          LED or indicator connected to PA1 (output).
 *          Internal pull-up resistor enabled on PA0 to ensure stable HIGH state when no object.
 *          This code uses direct register access (bare-metal).
 *
 * @usage   Connect IR sensor output to PA0.
 *          Connect LED to PA1.
 *          When the IR sensor detects an object (output LOW), the LED turns ON.
 *          When no object is detected (output HIGH), the LED remains OFF.
 ******************************************************************************
 */

/**
 ******************************************************************************
  Name : Monish Kumar.k
  Date : 29/12/2024
  File : interface_Ir_Sensor
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
void ir_Interface(void);
void delay(int ms);

int main()
{
	rcc_Config();
	choose_Port();
	gpio_Moder();
	while(1)
	{
		ir_Interface();
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
	//pull_up_ir_Interface
	*GPIOA_PUPDR = *GPIOA_PUPDR | (1<<0);//internal pull up
}

void delay(int ms)
{
	for(int i=0;i<ms*4000; i++)
	{
		__asm("NOP");
	}
}

void ir_Interface()
{
	if(*GPIOA_IDR & (0x00000001))
	{
		*GPIOA_ODR = *GPIOA_ODR & (~1<<1);
	}
	else if(*GPIOA_IDR & ~(0x00000001))
	{
		*GPIOA_ODR = *GPIOA_ODR | (1<<1);
		delay(1000);
	}
}
