/*
 * Timer2_interface.h
 *
 *  Created on: Mar 6, 2024
 *      Author: philo
 */

#ifndef TIMER2_INTERFACE_H_
#define TIMER2_INTERFACE_H_

#define   GPTIM_CEN           0
#define   GPTIM_UDIS          1
#define   GPTIM_URS           2
#define   GPTIM_OPM           3
#define   GPTIM_ARPE          7

#define   GPTIM_UIE           0

#define   GPTIM_UG            0

#define   GPTIM_UIF           0




void TIM2_voidInit(void);
void TIM2_voidSetBusyWait( u32 Copy_u32Ticks );
void TIM2_voidSetIntervalSingle  ( u32 Copy_u32Ticks, void (*Copy_ptr)(void) );
void TIM2_voidSetIntervalPeriodic( u32 Copy_u32Ticks, void (*Copy_ptr)(void) );
void TIM2_voidStopInterval(void);
u32  TIM2_u16GetElapsedTime(void);
u32  TIM2_u16GetRemainingTime(void);




#endif /* TIMER2_INTERFACE_H_ */
