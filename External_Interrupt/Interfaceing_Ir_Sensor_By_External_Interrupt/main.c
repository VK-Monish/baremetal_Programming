/**
 ******************************************************************************
 * @file    interfacing_Ir_Sensor_By_External_Interrupt.c
 * @author  Monish Kumar.K
 * @date    31/12/2024
 * @brief   IR Sensor Interfacing Using External Interrupt (EXTI) on STM32F4
 ******************************************************************************
 * @description
 * This bare-metal program interfaces an IR sensor with the STM32F4 microcontroller 
 * using an external interrupt on GPIO pin A0. When the sensor detects an object 
 * (falling edge trigger on PA0), an interrupt is generated. Upon handling the interrupt, 
 * a set of GPIOA pins (PA1 to PA8) is sequentially turned on with a delay to simulate 
 * a lighting or indication effect.
 * 
 * The main loop continuously turns on each LED (PA8 to PA1) one by one with delays.
 * On interrupt (triggered by the IR sensor), it performs another LED sequence and then resets.
 * 
 * @peripherals used:
 * - GPIOA (PA0 input with pull-up for EXTI, PA1 to PA8 as outputs)
 * - EXTI0 (for IR sensor interrupt detection)
 * - NVIC (to enable EXTI0 IRQ)
 * 
 * @registers used:
 * - RCC_CR, RCC_CFGR, RCC_AHB1ENR, RCC_APB2ENR
 * - GPIOA_MODER, GPIOA_ODR, GPIOA_PUPDR
 * - SYSCFG_EXTICR1, EXTI_IMR, EXTI_RTSR, EXTI_PR, NVIC_ISER0
 * 
 * @notes:
 * - Ensure the IR sensor is connected to PA0 and provides a falling edge on object detection.
 * - The delay function is based on NOPs and may vary based on system clock.
 * - LEDs (or other output devices) should be connected to PA1–PA8 for visual output.
 * 
 * @warning:
 * - No debounce mechanism for the IR signal.
 * - The code runs in an infinite loop with blocking delays.
 * - All configuration is done using register-level access; no HAL/LL drivers are used.
 ******************************************************************************
 */

/**
 ******************************************************************************
  Name : Monish Kumar.k
  Date : 31/12/2024
  File : interfacing_Ir_Sensor_By_External_Interrupt
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
void off();

int main()
{
	rcc_Config();
	choose_Port();
	gpio_Moder();
	exti_Config();
	while(1)
	{
		*GPIOA_ODR |= (1<<8);
		delay(300);
		*GPIOA_ODR |= (1<<7);
		delay(300);
		*GPIOA_ODR |= (1<<6);
		delay(300);
		*GPIOA_ODR |= (1<<5);
		delay(300);
		*GPIOA_ODR |= (1<<4);
		delay(300);
		*GPIOA_ODR |= (1<<3);
		delay(300);
		*GPIOA_ODR |= (1<<2);
		delay(300);
		*GPIOA_ODR |= (1<<1);
		delay(300);
		off();

	}
}

void rcc_Config()
{

	*RCC_CR = *RCC_CR & (~0x00010000); /*HES is diable */

	*RCC_CR = *RCC_CR | (1<<16); /*HES is Enable*/
        while(!(*RCC_CR & (1<<17)));
	//clock configuration

	*RCC_CFGR = *RCC_CFGR & (~0x00000003); /* disable the sw0 and sw1*/

	*RCC_CFGR = *RCC_CFGR | (1<<0);

	while(!(*RCC_CFGR & (1<<4)));
}

void choose_Port()
{
	*RCC_AHB1ENR  |=  (1<<0);
	*RCC_APB2ENR  |=  (1<<14);
}

void gpio_Moder()
{
	*GPIOA_MODER |=  (1<<2);
	*GPIOA_MODER |=  (1<<4);
	*GPIOA_MODER |=  (1<<6);
	*GPIOA_MODER |=  (1<<8);
	*GPIOA_MODER |=  (1<<10);
	*GPIOA_MODER |=  (1<<12);
	*GPIOA_MODER |=  (1<<14);
	*GPIOA_MODER |=  (1<<16);
	*GPIOA_MODER |=  (1<<18);
	*GPIOA_PUPDR = (*GPIOA_PUPDR & ~(0x3 << 0)) | (1 << 0);
}

void exti_Config()
{
	*SYSCFG_EXTICR1 = (*SYSCFG_EXTICR1 & ~(0xF << 0)) | (0<<0);

	*EXTI_IMR   |= (1<<0);

	*EXTI_RTSR  |= (1<<0);

	*NVIC_ISER0 |= (1<<6);
}

void off()
{
	*GPIOA_ODR &= ~(1<<1);
	*GPIOA_ODR &= ~(1<<2);
	*GPIOA_ODR &= ~(1<<3);
	*GPIOA_ODR &= ~(1<<4);
	*GPIOA_ODR &= ~(1<<5);
	*GPIOA_ODR &= ~(1<<6);
	*GPIOA_ODR &= ~(1<<7);
	*GPIOA_ODR &= ~(1<<8);
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
		for(int i=0; i<1; i++)
		{
			off();
			*GPIOA_ODR |= (1<<1);
			delay(500);
			*GPIOA_ODR |= (1<<2);
			delay(500);
			*GPIOA_ODR |= (1<<3);
			delay(500);
			*GPIOA_ODR |= (1<<4);
			delay(500);
			*GPIOA_ODR |= (1<<5);
			delay(500);
			*GPIOA_ODR |= (1<<6);
			delay(500);
			*GPIOA_ODR |= (1<<7);
			delay(500);
			*GPIOA_ODR |= (1<<8);
			delay(1000);
			off();
		}
	   *EXTI_PR |= (1<<0);
	}
}
