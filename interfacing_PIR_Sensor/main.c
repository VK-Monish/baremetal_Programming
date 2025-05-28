/**
 ******************************************************************************
 * @file    interface_Pir_Sensor.c
 * @author  Monish Kumar.k
 * @date    29/12/2024
 * @brief   Interface a PIR (Passive Infrared) sensor with STM32 using GPIO.
 *
 *          Description:
 *          - Enables High-Speed External (HSE) clock and selects it as system clock.
 *          - Enables clock for GPIO Port A and Port C.
 *          - Configures PA0 as input with internal pull-up resistor enabled for PIR sensor input.
 *          - Configures PA4 as output (assumed LED or indicator) on GPIOA.
 *          - Configures PC13 as output (commonly onboard LED on many STM32 boards).
 *          - Continuously reads the PIR sensor input on PA0.
 *          - Turns OFF the LED on PC13 when PIR sensor input is HIGH (motion detected).
 *          - Turns ON the LED on PC13 when PIR sensor input is LOW (no motion).
 *
 * @note    PIR sensor output is connected to PA0 (input).
 *          Indicator LED connected to PC13 (output).
 *          Internal pull-up enabled on PA0 to stabilize input.
 *          Direct register access (bare-metal) is used.
 *
 * @usage   Connect PIR sensor output to PA0.
 *          Connect an LED to PC13 with appropriate current limiting resistor.
 *          When PIR sensor detects motion (PA0 HIGH), the LED on PC13 will turn OFF.
 *          When no motion is detected (PA0 LOW), LED on PC13 will turn ON.
 ******************************************************************************
 */


/**
 ******************************************************************************
  Name : Monish Kumar.k
  Date : 29/12/2024
  File : interface_Pir_Sensor
 ******************************************************************************/
volatile unsigned int *RCC_CR      = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_CFGR    = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;

volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000;
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014;
volatile unsigned int *GPIOA_IDR   = (volatile unsigned int *)0x40020010;
volatile unsigned int *GPIOA_PUPDR = (volatile unsigned int *)0x4002000C;

volatile unsigned int *GPIOC_MODER = (volatile unsigned int *)0x40020800;
volatile unsigned int *GPIOC_ODR   = (volatile unsigned int *)0x40020814;

void rcc_Config(void);
void choose_Port_A(void);
void gpio_Moder(void);
void pir_interface(void);
void delay_Ms(int ms);

int main(void)
{
	rcc_Config();
	choose_Port_A();
	gpio_Moder();
	while(1)
	{
		pir_interface();
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
	*RCC_AHB1ENR = *RCC_AHB1ENR | (1<<2);
}

void gpio_Moder()
{
	*GPIOA_MODER = *GPIOA_MODER | (1<<8);
	*GPIOA_MODER = *GPIOA_MODER & (~0x00000003);
	*GPIOA_PUPDR = *GPIOA_PUPDR | (1<<1);
	*GPIOC_MODER = *GPIOC_MODER | (1<<26);
}

void delay_Ms(int ms)
{
	for(int i=0; i<ms*4000; i++)
	{
		__asm("NOP");
	}
}
void pir_interface()
{
	if((*GPIOA_IDR & (0X00000001)) == 1)
	{
		*GPIOC_ODR = *GPIOC_ODR & ~(1<<13);
	}
	else
	{
		*GPIOC_ODR = *GPIOC_ODR | (1<<13);
	}
}
