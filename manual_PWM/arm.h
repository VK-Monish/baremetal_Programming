/*
 * arm.h
 *
 *  Created on: Jan 16, 2025
 *      Author: moni
 */

#ifndef ARM_H_
#define ARM_H_

struct rcc
{
	unsigned int CR;
	unsigned int PLLCFGR;
	unsigned int CFGR;
	unsigned int CIR;
	unsigned int AHB1RSTR;
	unsigned int AHB2RSTR;
	unsigned int res1;
	unsigned int res2;
	unsigned int APB1RSTR;
	unsigned int APB2RSTR;
	unsigned int res3;
	unsigned int res4;
	unsigned int AHB1ENR;
	unsigned int AHB2ENR;
	unsigned int res13;
	unsigned int res14;
	unsigned int APB1ENR;
	unsigned int APB2ENR;
};
volatile struct rcc *RCC = (volatile struct rcc*)0x40023800;

struct timer
{
	unsigned int CR1;		//cr1   0X00
	unsigned int CR2;	    //cr2   0X04
	unsigned int SMCR;		//SMCR  0X08
	unsigned int DIER;		//DIER  0X0C
	unsigned int SR;		//SR    0X10
	unsigned int EGR;		//ECGR  0X14
	unsigned int CCMR1;     //CCMR1 0X18
	unsigned int res5;		//res5  0X1c
	unsigned int CCER;      //CCER  0X20
	unsigned int CNT;       //CNT 	0X24
	unsigned int PSC;       //PSC   0X28
	unsigned int ARR;		//ARR   0X2C
	unsigned int res6;		//res6  0x30
	unsigned int CCR1;		//CCR1  0x34
	unsigned int res7;		//res7  0x38
	unsigned int res8;      //res8  0x3c
	unsigned int res9;		//res9  0x40
	unsigned int res10;		//res10 0x44
	unsigned int res11;		//res11 0x48
	unsigned int res12;     //res12 0x4c
	unsigned int OR;		//OR    0x50
};

volatile struct timer *TIM10 = (volatile struct timer*)0x40014400;

struct gpio
{
	unsigned int MODER;
	unsigned int OTYPER;
	unsigned int OSPEEDR;
	unsigned int PUPDR;
	unsigned int IDR;
	unsigned int ODR;
	unsigned int BSRR;
	unsigned int LCKR;
	unsigned int AFRL;
	unsigned int AFRH;
};

volatile struct gpio *GPIOC = (volatile struct gpio*)0x40020800;

#endif /* ARM_H_ */
