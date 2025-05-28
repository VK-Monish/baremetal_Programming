/**
 ******************************************************************************
 * Project Title   : 8x8 LED Pattern Display using Pull-up and Pull-down Buttons
 * File Name       : 8x8_Led_Pullup_pullDown.c
 * Author          : Monish Kumar K.
 * Date            : 27/12/2024
 * Target Platform : STM32F4xx Series (Bare-Metal)
 *
 * Description:
 * --------------
 * This bare-metal embedded C program controls an 8x8 LED matrix using GPIOA and GPIOB
 * on an STM32F4 microcontroller. It uses internal pull-up and pull-down configurations
 * on GPIOB input pins to detect button presses and displays specific LED patterns accordingly.
 *
 * Functional Overview:
 * ---------------------
 * - Initializes system clock using HSE.
 * - Enables GPIOA and GPIOB peripheral clocks.
 * - Configures GPIOA and GPIOB pins as output for LED control.
 * - Monitors GPIOB inputs (configured as pull-up and pull-down) for button states.
 * - If pull-up pin (PB12) is high (button not pressed), LEDs are OFF.
 * - If pull-up pin is low (button pressed), `pattern0()` is shown.
 * - If pull-down pin (PB14) is high (button pressed), `pattern1()` is shown.
 *
 * Pin Usage:
 * ----------
 * - GPIOA Pins 0 to 7 : Row control for 8x8 LED matrix.
 * - GPIOB Pins 0, 1, 2, 5, 6, 7, 8, 9 : Column control for 8x8 LED matrix.
 * - GPIOB Pin 12 : Pull-up button input.
 * - GPIOB Pin 14 : Pull-down button input.
 *
 * Functions:
 * ----------
 * - main()                : Initializes peripherals and loops button scan.
 * - rcc_Config()          : Configures system clock.
 * - choose_Port_A()       : Enables clocks for GPIOA and GPIOB.
 * - gpio_Moder()          : Configures basic GPIOB outputs.
 * - gpio_Moder_Pattern()  : Configures both GPIOA and GPIOB for LED pattern display.
 * - delay_Ms()            : Software delay loop (approximate timing).
 * - off_All()             : Turns off all LEDs.
 * - pattern0()            : Turns on LEDs in a progressive forward pattern.
 * - pattern1()            : Turns on LEDs in a reverse pattern.
 * - button_Config()       : Checks button state and invokes appropriate LED pattern.
 *
 * How to Build and Flash:
 * ------------------------
 * 1. Compile using `arm-none-eabi-gcc` or with Makefile.
 * 2. Flash using OpenOCD or ST-Link utility.
 *
 * Notes:
 * ------
 * - This is a bare-metal implementation and does not use any HAL or CMSIS libraries.
 * - Make sure your circuit has proper resistors for LEDs and buttons.
 * - You can modify the pin assignments and delay logic for your own hardware setup.
 *
 ******************************************************************************
 */

/**
 ******************************************************************************
  Name : Monish Kumar.k
  Date : 27/12/2024
  File : 8x8_Led_Pullup_pullDown
 ******************************************************************************/
volatile unsigned int *RCC_CR      = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_CFGR    = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;

volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000;
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014;
volatile unsigned int *GPIOA_IDR   = (volatile unsigned int *)0x40020010;

volatile unsigned int *GPIOB_MODER = (volatile unsigned int *)0x40020400;
volatile unsigned int *GPIOB_ODR   = (volatile unsigned int *)0x40020414;
volatile unsigned int *GPIOB_IDR   = (volatile unsigned int *)0x40020410;

void rcc_Config(void);
void choose_Port_A(void);
void gpio_Moder(void);
void button_Config(void);
void pattern0(void);
void off_All(void);
void pattern1(void);
void gpio_Moder_Pattern(void);
void delay_Ms(int ms);

int main(void)
{
	rcc_Config();
	choose_Port_A();
	gpio_Moder();
	gpio_Moder_Pattern();
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
	*RCC_AHB1ENR = *RCC_AHB1ENR | (1<<1);
}

void gpio_Moder()
{
	*GPIOB_MODER = *GPIOB_MODER | (1<<26);
	*GPIOB_MODER = *GPIOB_MODER & (~0x03000000);
	*GPIOB_MODER = *GPIOB_MODER & (~0x30000000);
	*GPIOB_MODER = *GPIOB_MODER | (1<<30);
}

void gpio_Moder_Pattern()
{
	int j=10;
	for(int i=0 ; i<=14; i=i+2)
	{
		*GPIOA_MODER = *GPIOA_MODER | (1<<i);
		if(i>=6)
		{
			*GPIOB_MODER = *GPIOB_MODER | (1<<j);
			j=j+2;
		}
		else
		{
			*GPIOB_MODER = *GPIOB_MODER | (1<<i);
		}
	}
}

void delay_Ms(int ms)
{
	for(int i=0; i<ms*4000; i++)
	{
		__asm("NOP");
	}
}
void off_All()
{
	for(int i=0; i<8; i++)
	{
		*GPIOA_ODR = *GPIOA_ODR & (~1<<i);
	}
	*GPIOB_ODR = *GPIOB_ODR & (~1<<0);
	*GPIOB_ODR = *GPIOB_ODR & (~1<<1);
	*GPIOB_ODR = *GPIOB_ODR & (~1<<2);
	*GPIOB_ODR = *GPIOB_ODR & (~1<<5);
	*GPIOB_ODR = *GPIOB_ODR & (~1<<6);
	*GPIOB_ODR = *GPIOB_ODR & (~1<<7);
	*GPIOB_ODR = *GPIOB_ODR & (~1<<8);
	*GPIOB_ODR = *GPIOB_ODR & (~1<<9);
}

void pattern0()
{
	for(int i=0; i<8; i++)
	{
		*GPIOA_ODR = *GPIOA_ODR | (1<<i);
	}
	*GPIOB_ODR = *GPIOB_ODR | (1<<0);
	delay_Ms(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<1);
	delay_Ms(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<2);
	delay_Ms(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<5);
	delay_Ms(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<6);
	delay_Ms(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<7);
	delay_Ms(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<8);
	delay_Ms(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<9);
	delay_Ms(100);
}

void pattern1()
{
	for(int i=0; i<8; i++)
	{
		*GPIOA_ODR = *GPIOA_ODR | (1<<i);
	}
	*GPIOB_ODR = *GPIOB_ODR | (1<<9);
	delay_Ms(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<8);
	delay_Ms(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<7);
	delay_Ms(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<6);
	delay_Ms(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<5);
	delay_Ms(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<2);
	delay_Ms(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<1);
	delay_Ms(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<0);
	delay_Ms(100);
}

void button_Config()
{
	//Pull_UP
	if((*GPIOB_IDR & (0X00001000)))
	{
		off_All();
	}
	else
	{
		pattern0();
		off_All();
	}
	//pULL_Down
	if((*GPIOB_IDR & (0X00004000)))
	{
		pattern1();
		off_All();
	}
	else
	{
		off_All();
	}
}
