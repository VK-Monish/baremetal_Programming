/**
 ******************************************************************************
 * @file    three_external_Interrupt.c
 * @author  Monish Kumar K
 * @date    03/01/2025
 * @brief   Example of configuring and handling three external interrupts (EXTI)
 *          on STM32F4 using bare-metal programming without HAL libraries.
 *
 * @details
 * This program configures three external interrupts on GPIOA pins PA0, PA1, and PA15.
 * Each interrupt triggers an ISR that blinks an LED connected to PA6, PA5, or PA7 respectively.
 *
 * GPIO Pin usage:
 *  - PA0  : EXTI0 input interrupt (blinks LED on PA6)
 *  - PA1  : EXTI1 input interrupt (blinks LED on PA5)
 *  - PA15 : EXTI15 input interrupt (blinks LED on PA7)
 *  - PA5, PA6, PA7 : Output pins to drive LEDs
 *
 * Core functionalities:
 *  - RCC clock configuration for GPIOA and SYSCFG peripherals
 *  - GPIO mode and pull-up/down resistor setup
 *  - SYSCFG external interrupt configuration for pin mapping
 *  - EXTI interrupt mask and trigger selection (rising edge)
 *  - NVIC interrupt enabling for EXTI lines 0, 1, and 15
 *  - Interrupt service routines for each EXTI line with LED blinking logic
 *
 * Note:
 *  - External triggers (e.g., buttons) must be connected to PA0, PA1, and PA15 pins.
 *  - Delay function uses a simple loop with NOP instructions for timing.
 ******************************************************************************
 */


/**
 ******************************************************************************
  Name : Monish Kumar.k
  Date : 3/01/2025
  File : three_external_Interrupt
 ******************************************************************************/
 
volatile unsigned int *RCC_CR         = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_CFGR       = (volatile unsigned int *)0x40023808;

volatile unsigned int *RCC_AHB1ENR    = (volatile unsigned int *)0x40023830;
volatile unsigned int *RCC_APB2ENR    = (volatile unsigned int *)0x40023844;

volatile unsigned int *GPIOA_MODER    = (volatile unsigned int *)0x40020000;
volatile unsigned int *GPIOA_ODR      = (volatile unsigned int *)0x40020014;
volatile unsigned int *GPIOA_PUPDR    = (volatile unsigned int *)0x4002000C;

volatile unsigned int *EXTI_IMR       = (volatile unsigned int *)0x40013C00;
volatile unsigned int *EXTI_RTSR      = (volatile unsigned int *)0x40013C08;
volatile unsigned int *EXTI_PR        = (volatile unsigned int *)0x40013C14;

volatile unsigned int *SYSCFG_EXTICR1 = (volatile unsigned int *)0x40013808;
volatile unsigned int *SYSCFG_EXTICR4 = (volatile unsigned int *)0x40013814;
volatile unsigned int *NVIC_ISER0     = (volatile unsigned int *)0xE000E100;
volatile unsigned int *NVIC_ISER1     = (volatile unsigned int *)0xE000E104;

void rcc_Config(void);
void choose_Port(void);
void gpio_Moder(void);
void exti_Config(void);
void delay(int ms);
void EXTI1_IRQHandler(void);
void EXTI15_10_IRQHandler (void);
void EXTI0_IRQHandler(void);

int main()
{
    rcc_Config();
    choose_Port();
    gpio_Moder();
    exti_Config();
}

void rcc_Config()
{
    *RCC_CR = *RCC_CR & (~0x00010000);
    *RCC_CR = *RCC_CR | (1 << 16);
    *RCC_CFGR = *RCC_CFGR & (~0x00000003);
    *RCC_CFGR = *RCC_CFGR | (1 << 0);
}

void choose_Port()
{
    *RCC_AHB1ENR |= (1 << 0);
    *RCC_APB2ENR |= (1 << 14);
}

void gpio_Moder()
{
    *GPIOA_MODER |= (1 << 10);
    *GPIOA_MODER &= ~(1 << 11);

    *GPIOA_MODER |= (1 << 12);
    *GPIOA_MODER &= ~(1 << 13);

    *GPIOA_MODER |= (1<<14);
    *GPIOA_MODER &= ~(1<<15);

    *GPIOA_MODER &= ~(1 << 2);
    *GPIOA_MODER &= ~(1 << 3);

    *GPIOA_MODER &= ~(1 << 0);
    *GPIOA_MODER &= ~(1 << 1);

    *GPIOA_MODER &= ~(1<<30);
    *GPIOA_MODER &= ~(1<<31);

    *GPIOA_PUPDR |= (1<<1);
    *GPIOA_PUPDR &= ~(1<<0);//PULL DOWN

    *GPIOA_PUPDR |= (1 << 2);
    *GPIOA_PUPDR &= ~(1 << 3);//PULL UP

    *GPIOA_PUPDR |= (1<<31);
    *GPIOA_PUPDR &= ~(1<<30);// PULL DOWN
}

void exti_Config()
{
	//FOR  PINA1 PIN
    *SYSCFG_EXTICR1 = (*SYSCFG_EXTICR1 & ~(0xF << 4)) | (0 << 4);
    *EXTI_IMR |= (1 << 1);
    *EXTI_RTSR |= (1 << 1);
    *NVIC_ISER0 |= (1 << 7);

    //FOR A0 PIN
    *SYSCFG_EXTICR1 = (*SYSCFG_EXTICR1 & ~(0xF << 0)) | (0 << 0);
    *EXTI_IMR |= (1 << 0);
    *EXTI_RTSR |= (1 << 0);
    *NVIC_ISER0 |= (1 << 6);

    //A15
    *SYSCFG_EXTICR4 = (*SYSCFG_EXTICR4 & ~(0XF << 12)) | (0<<12);
    *EXTI_IMR |= (1<<15);
    *EXTI_RTSR |= (1 << 15);
    *NVIC_ISER1 |= (1 << 8);

}

void delay(int ms)
{
    for (int i = 0; i < ms * 4000; i++)
    {
        __asm("NOP");
    }
}

void EXTI0_IRQHandler()
{
    if (*EXTI_PR & (1 << 0))
    {
        for (int i = 0; i < 5; i++)
        {
            *GPIOA_ODR &= ~(1 << 6);
            delay(100);
            *GPIOA_ODR |= (1 << 6);  // Turn on LED
            delay(100);
        }
        *GPIOA_ODR &= ~(1 << 6);
        *EXTI_PR |= (1 << 0);
    }
}

void EXTI1_IRQHandler()
{
    if (*EXTI_PR & (1 << 1))
    {
        for (int i = 0; i < 5; i++)
        {
            *GPIOA_ODR &= ~(1 << 5);
            delay(100);
            *GPIOA_ODR |= (1 << 5);  // Turn on LED
            delay(100);
        }
        *GPIOA_ODR &= ~(1 << 5);
        *EXTI_PR |= (1 << 1);
    }
}
void EXTI15_10_IRQHandler ()
{
    if (*EXTI_PR & (1 << 15))
    {
        for (int i = 0; i < 5; i++)
        {
            *GPIOA_ODR &= ~(1 << 7);
            delay(100);
            *GPIOA_ODR |= (1 << 7); // Turn on LED
            delay(100);
        }
        *GPIOA_ODR &= ~(1 << 7);
        *EXTI_PR |= (1 << 15);
    }
}
