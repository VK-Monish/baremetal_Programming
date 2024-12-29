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
