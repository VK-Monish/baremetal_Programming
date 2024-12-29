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
