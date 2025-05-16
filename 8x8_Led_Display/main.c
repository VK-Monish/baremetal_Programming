/**
 ******************************************************************************
 * @file    8x8_Led_Display.c
 * @author  Monish Kumar.k
 * @date    25/12/2024
 * @brief   STM32 bare-metal program to control an 8x8 LED matrix display
 *          using GPIO ports A and B.
 *
 * Description:
 * This program configures the STM32 microcontroller GPIO pins to drive an 8x8 LED
 * matrix display. It performs clock configuration, enables GPIO ports A and B,
 * sets pin modes, and demonstrates different LED patterns by manipulating the
 * output data registers (ODR) of the GPIO ports.
 *
 * Features:
 * - RCC (Reset and Clock Control) configuration to enable HSE clock and GPIO clocks.
 * - GPIO mode configuration for pins used in the LED matrix.
 * - Two main LED patterns displayed sequentially with delays.
 * - Functions to turn off all LEDs before switching patterns.
 * - Uses busy-wait loop for delays.
 *
 * Hardware Connections:
 * - GPIOA pins 0 to 7 connected to one set of LED rows or columns.
 * - GPIOB pins 0,1,2,5,6,7,8,9 connected to the other set of LED rows or columns.
 *
 * Usage:
 * - Compile and flash this code to your STM32 microcontroller.
 * - The program will continuously cycle through LED patterns on the 8x8 matrix.
 *
 * Notes:
 * - This is a bare-metal program without any HAL or driver libraries.
 * - Delay is implemented with a simple busy-wait loop.
 * - Modify GPIO pins as needed based on your hardware wiring.
 *
 * References:
 * - STM32F4xx Reference Manual for register details.
 *
 ******************************************************************************
 */


volatile unsigned int *RCC_CR      = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_CFGR    = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;

volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000;
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014;

volatile unsigned int *GPIOB_MODER = (volatile unsigned int *)0x40020400;
volatile unsigned int *GPIOB_ODR   = (volatile unsigned int *)0x40020414;

void rcc_Config(void);
void choose_Port_A_C(void);
void gpio_Moder(void);
void pattern_0(void);
void pattern_1(void);
void pattern_2(void);
void patterns(void);
void delay(int ms);
void off_All(void);

int main(void)
{
	rcc_Config();
	choose_Port_A_C();
	gpio_Moder();
	while(1)
	{
		patterns();
	}
}

void rcc_Config()
{
	*RCC_CR = *RCC_CR & (~0x00010000); /*HES is diable */

	*RCC_CR = *RCC_CR | (1<<16); /*HES is Enable*/

	//while(!(*RCC_CR & (1<<17))==0);

	//clock configuration

	*RCC_CFGR = *RCC_CFGR & (~0x00000003); /* disable the sw0 and sw1*/

	*RCC_CFGR = *RCC_CFGR | (1<<0);

	//while(!(*RCC_CFGR & (1<<4)));

}

void choose_Port_A_C()
{
	*RCC_AHB1ENR = *RCC_AHB1ENR | (1<<0);
	*RCC_AHB1ENR = *RCC_AHB1ENR | (1<<1);
}

void gpio_Moder()
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

void delay(int ms)
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
		*GPIOA_ODR = *GPIOA_ODR & ~(1<<i);
	}
	*GPIOB_ODR = *GPIOB_ODR & ~(1<<0);
	*GPIOB_ODR = *GPIOB_ODR & ~(1<<1);
	*GPIOB_ODR = *GPIOB_ODR & ~(1<<2);
	*GPIOB_ODR = *GPIOB_ODR & ~(1<<5);

	*GPIOB_ODR = *GPIOB_ODR & ~(1<<6);
	*GPIOB_ODR = *GPIOB_ODR & ~(1<<7);
	*GPIOB_ODR = *GPIOB_ODR & ~(1<<8);
	*GPIOB_ODR = *GPIOB_ODR & ~(1<<9);
}
void patterns()
{
	pattern_0();
	off_All();
	pattern_1();
	off_All();
}

void pattern_0()
{
	for(int i=0; i<8; i++)
	{
		*GPIOA_ODR = *GPIOA_ODR | (1<<i);// portA,pin 0 to 8
	}
	*GPIOB_ODR = *GPIOB_ODR | (1<<0);//pin 0
	delay(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<1);//pin 1
	delay(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<2);//pin 2
	delay(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<5);//pin 3
	delay(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<6);//pin 4
	delay(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<7);//pin 5
	delay(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<8);//pin 6
	delay(100);
	*GPIOB_ODR = *GPIOB_ODR | (1<<9);//pin 7
	delay(500);
}

void pattern_1()
{
	for(int i=0; i<8; i++)
	{
		*GPIOA_ODR = *GPIOA_ODR | (1<<i);
	}
	*GPIOB_ODR = *GPIOB_ODR | (1<<0);
	delay(100);
	*GPIOB_ODR = *GPIOB_ODR & ~(1<<0);
	*GPIOB_ODR = *GPIOB_ODR | (1<<1);
	delay(100);
	*GPIOB_ODR = *GPIOB_ODR & ~(1<<1);
	*GPIOB_ODR = *GPIOB_ODR | (1<<2);
	delay(100);
	*GPIOB_ODR = *GPIOB_ODR & ~(1<<2);
	*GPIOB_ODR = *GPIOB_ODR | (1<<5);
	delay(100);
	*GPIOB_ODR = *GPIOB_ODR & ~(1<<5);
	*GPIOB_ODR = *GPIOB_ODR | (1<<6);
	delay(100);
	*GPIOB_ODR = *GPIOB_ODR & ~(1<<6);
	*GPIOB_ODR = *GPIOB_ODR | (1<<7);
	delay(100);
	*GPIOB_ODR = *GPIOB_ODR & ~(1<<7);
	*GPIOB_ODR = *GPIOB_ODR | (1<<8);
	delay(100);
	*GPIOB_ODR = *GPIOB_ODR & ~(1<<8);
	*GPIOB_ODR = *GPIOB_ODR | (1<<9);
	delay(100);
	*GPIOB_ODR = *GPIOB_ODR & ~(1<<9);
	delay(100);
}
