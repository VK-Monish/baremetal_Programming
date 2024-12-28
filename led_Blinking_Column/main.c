/**
 ******************************************************************************
  Name : Monish Kumar.k
  Date : 24/12/2024
  File : led_Blinking_Column
 ******************************************************************************/
volatile unsigned int *RCC_CR      = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_CFGR    = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;

volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000;
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014;

void rcc_Config(void);
void choose_Port_A(void);
void gpio_Moder(void);
void led_Blink_C13(void);
void delay_Ms(int ms);

int main(void)
{
	rcc_Config();
	choose_Port_C();
	gpio_Moder();
	while(1)
	{
		led_Blink_C13();
	}
}

void rcc_Config()
{
	*RCC_CR = *RCC_CR & (~0x00010000); /*HES is diable */

	*RCC_CR = *RCC_CR | (1<<16); /*HES is Enable*/

	while(!(*RCC_CR & (1<<17))==0);

	//clock configuration

	*RCC_CFGR = *RCC_CFGR & (~0x00000003); /* disable the sw0 and sw1*/

	*RCC_CFGR = *RCC_CFGR | (1<<0);

	while(!(*RCC_CFGR & (1<<4))==4);

}

void choose_Port_A()
{
	*RCC_AHB1ENR = *RCC_AHB1ENR | (1<<0);
}

void gpio_Moder()
{
	*GPIOA_MODER = *GPIOA_MODER | (1<<0);
	*GPIOA_MODER = *GPIOA_MODER | (1<<2);
	*GPIOA_MODER = *GPIOA_MODER | (1<<4);
	*GPIOA_MODER = *GPIOA_MODER | (1<<6);
	*GPIOA_MODER = *GPIOA_MODER | (1<<8);
	*GPIOA_MODER = *GPIOA_MODER | (1<<10);
	*GPIOA_MODER = *GPIOA_MODER | (1<<12);
	*GPIOA_MODER = *GPIOA_MODER | (1<<14);
}

void delay_Ms(int ms)
{
	for(int i=0; i<ms*4000; i++)
	{
		__asm("NOP");
	}
}
void led_Blink_C13()
{
	*GPIOA_ODR = *GPIOA_ODR | (1<<0); // LED on
	delay_Ms(100);
	*GPIOA_ODR = *GPIOA_ODR & ~(1<<0);// LED off
	*GPIOA_ODR = *GPIOA_ODR | (1<<1); // LED on
	delay_Ms(100);
	*GPIOA_ODR = *GPIOA_ODR & ~(1<<1);// LED off
	*GPIOA_ODR = *GPIOA_ODR | (1<<2); // LED on
	delay_Ms(100);
	*GPIOA_ODR = *GPIOA_ODR & ~(1<<2);// LED off
	*GPIOA_ODR = *GPIOA_ODR | (1<<3); // LED on
	delay_Ms(100);
	*GPIOA_ODR = *GPIOA_ODR & ~(1<<3);// LED off
	*GPIOA_ODR = *GPIOA_ODR | (1<<4); // LED on
	delay_Ms(100);
	*GPIOA_ODR = *GPIOA_ODR & ~(1<<4);// LED off
	*GPIOA_ODR = *GPIOA_ODR | (1<<5); // LED on
	delay_Ms(100);
	*GPIOA_ODR = *GPIOA_ODR & ~(1<<5);// LED off
	*GPIOA_ODR = *GPIOA_ODR | (1<<6); // LED on
	delay_Ms(100);
	*GPIOA_ODR = *GPIOA_ODR & ~(1<<6);// LED off
	*GPIOA_ODR = *GPIOA_ODR | (1<<7); // LED on
	delay_Ms(100);
	*GPIOA_ODR = *GPIOA_ODR & ~(1<<7);// LED off
	delay_Ms(100);

	*GPIOA_ODR = *GPIOA_ODR | (0x00000002); // LED on
	*GPIOA_ODR = *GPIOA_ODR | (0x00000008); // LED on
	*GPIOA_ODR = *GPIOA_ODR | (0x00000020); // LED on
	*GPIOA_ODR = *GPIOA_ODR | (0x00000080); // LED on
	delay_Ms(700);
	*GPIOA_ODR = *GPIOA_ODR & ~(0x00000002);// LED off
	*GPIOA_ODR = *GPIOA_ODR & ~(0x00000008);// LED off
	*GPIOA_ODR = *GPIOA_ODR & ~(0x00000020);// LED off
	*GPIOA_ODR = *GPIOA_ODR & ~(0x00000080);// LED off
	delay_Ms(700);
	*GPIOA_ODR = *GPIOA_ODR | (0x00000001); // LED on
	*GPIOA_ODR = *GPIOA_ODR | (0x00000004); // LED on
	*GPIOA_ODR = *GPIOA_ODR | (0x00000010); // LED on
	*GPIOA_ODR = *GPIOA_ODR | (0x00000040); // LED on
	delay_Ms(700);
	*GPIOA_ODR = *GPIOA_ODR & ~(0x00000001);// LED off
	*GPIOA_ODR = *GPIOA_ODR & ~(0x00000004);// LED off
	*GPIOA_ODR = *GPIOA_ODR & ~(0x00000010);// LED off
	*GPIOA_ODR = *GPIOA_ODR & ~(0x00000040);// LED off
	delay_Ms(700);


	*GPIOA_ODR = *GPIOA_ODR | (0b00000000000000000000000000001111);
	delay_Ms(700);
	*GPIOA_ODR = *GPIOA_ODR & (~0b0000000000000000000000000001111);
	delay_Ms(700);
	*GPIOA_ODR = *GPIOA_ODR | (0b00000000000000000000000011110000);
	delay_Ms(700);
	*GPIOA_ODR = *GPIOA_ODR & (~0b0000000000000000000000011110000);
	delay_Ms(700);
}
