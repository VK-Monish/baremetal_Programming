/**
 ******************************************************************************
  Name : Monish Kumar.k
  Date : 30/12/2024
  File : external_Interrupt_A0_Pin
 ******************************************************************************/
volatile unsigned int *RCC_CR         = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_CFGR       = (volatile unsigned int *)0x40023808;

volatile unsigned int *RCC_AHB1ENR    = (volatile unsigned int *)0x40023830;
volatile unsigned int *RCC_APB2ENR    = (volatile unsigned int *)0x40023844;

volatile unsigned int *GPIOA_MODER    = (volatile unsigned int *)0x40020000;
volatile unsigned int *GPIOA_ODR      = (volatile unsigned int *)0x40020014;
volatile unsigned int *GPIOA_PUPDR    = (volatile unsigned int *)0x4002000C;

//interrupt register......
volatile unsigned int *EXTI_IMR       = (volatile unsigned int *)0x40013C00;
volatile unsigned int *EXTI_RTSR      = (volatile unsigned int *)0x40013C08;
volatile unsigned int *EXTI_PR        = (volatile unsigned int *)0x40013C14;

volatile unsigned int *SYSCFG_EXTICR1 = (volatile unsigned int *)0x40013808;
volatile unsigned int *NVIC_ISER0     = (volatile unsigned int *)0xE000E100;

void rcc_Config(void);
void choose_Port(void);
void gpio_Moder(void);
void exti_Config(void);
void delay(int ms);
void EXTI0_IRQHandler(void);

int main()
{
	rcc_Config();
	choose_Port();
	gpio_Moder();
	exti_Config();
	while(1)
	{
		*GPIOA_ODR |= (1<<5);
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
	*RCC_AHB1ENR  |=  (1<<0);
	*RCC_APB2ENR  |=  (1<<14);
}

void gpio_Moder()
{
	*GPIOA_MODER = (*GPIOA_MODER & ~(0x3 << 0)) | (1<<10);
	*GPIOA_PUPDR = (*GPIOA_PUPDR & ~(0x3 << 0)) | (1 << 0);
}

void exti_Config()
{
	*SYSCFG_EXTICR1 = (*SYSCFG_EXTICR1 & ~(0xF << 0)) | (0<<0);

	*EXTI_IMR   |= (1<<0);

	*EXTI_RTSR  |= (1<<0);

	*NVIC_ISER0 |= (1<<6);
}

void delay(int ms)
{
	for(int i=0;i<ms*4000;i++)
	{
		__asm("NOP");
	}
}
void EXTI0_IRQHandler()
{
	if(*EXTI_PR & (1<<0))
	{
		for(int i=0; i<5; i++)
		{
			*GPIOA_ODR &= ~(1<<5);
			delay(100);
			*GPIOA_ODR |= (1<<5);
			delay(100);
		}
	   *EXTI_PR |= (1<<0);
	}
}
