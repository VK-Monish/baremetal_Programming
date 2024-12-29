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