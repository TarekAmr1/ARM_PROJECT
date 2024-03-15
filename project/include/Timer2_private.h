/*
 * Timer2_private.h
 *
 *  Created on: Mar 6, 2024
 *      Author: philo
 */

#ifndef TIMER2_PRIVATE_H_
#define TIMER2_PRIVATE_H_

#define    SINGLE_INTERVAL     0
#define    PERIODIC_INTERVAL   1

static volatile  u32 Global2_u32Routine=0;
static volatile  u32 Global2_u32Fraction;
static volatile  u32 Global2_u32CopyRoutine;
static volatile  u8  Global2_u8Flag=1;

static volatile void (*TIM2_CallBack)(void);

static volatile u8 TIM2_u8ModeOfInterval;



typedef struct
{
	volatile u32 CR1;           /*00*/
	volatile u32 CR2;           /*04*/
	volatile u32 SMCR;          /*08*/
	volatile u32 DIER;          /*0C*/
	volatile u32 SR;            /*10*/
	volatile u32 EGR;           /*14*/
	volatile u32 CCMR1;         /*18*/
	volatile u32 CCMR2;         /*1C*/
	volatile u32 CCER;          /*20*/
	volatile u32 CNT;           /*24*/
	volatile u32 PSC;           /*28*/
	volatile u32 ARR;           /*2C*/
	volatile u32 Reserved1;     /*30*/
	volatile u32 CCR1;          /*34*/
	volatile u32 CCR2;          /*38*/
	volatile u32 CCR3;          /*3C*/
	volatile u32 CCR4;          /*40*/
	volatile u32 Reserved2;     /*44*/
	volatile u32 DCR;           /*48*/
	volatile u32 DMAR;          /*4C*/
}TIM_t;

#define  TIM2    ((volatile TIM_t*)0x40000000)


#endif /* TIMER2_PRIVATE_H_ */
