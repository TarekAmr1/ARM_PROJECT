/*
 * Timer3_interface.h
 *
 *  Created on: Mar 10, 2024
 *      Author: philo
 */

#ifndef TIMER3_INTERFACE_H_
#define TIMER3_INTERFACE_H_


#define   GPTIM_CEN           0
#define   GPTIM_UDIS          1
#define   GPTIM_URS           2
#define   GPTIM_OPM           3
#define   GPTIM_ARPE          7
/******************DIER***********************/
#define   GPTIM_UIE           0
/*******************EGR**************************/
#define   GPTIM_UG            0
/*******************SR***************************/
#define   GPTIM_UIF           0

void TIM3_voidInit(void);
void TIM3_voidSetBusyWait( u32 Copy_u32Ticks );
void TIM3_voidSetIntervalSingle  ( u32 Copy_u32Ticks, void (*Copy_ptr)(void) );
void TIM3_voidSetIntervalPeriodic( u32 Copy_u32Ticks, void (*Copy_ptr)(void) );
void TIM3_voidStopInterval(void);
u32  TIM3_u16GetElapsedTime(void);
u32  TIM3_u16GetRemainingTime(void);



#endif /* TIMER3_INTERFACE_H_ */
