/*
 * Timer2_prog.c
 *
 *  Created on: Mar 6, 2024
 *      Author: philo
 */
#include"STD_TYPES.h"
#include"BIT_MATH.h"
#include"Timer2_private.h"
#include"Timer2_config.h"
#include"Timer2_interface.h"



void TIM2_voidInit(void)
{
	/*Up Counter Mode*/
	TIM2->CR1&=~(0b111<<4);
	TIM2->CR1|=(DOWNCOUNTER<<4);

	/*ARR is Buffered can't changed Execpt at Update Event*/
	SET_BIT(TIM2->CR1,GPTIM_ARPE);

	/*Only Counter OverFlow UnderFlow Generate update Interrupt */
	SET_BIT(TIM2->CR1,GPTIM_URS);

	/*Enable Update Event */
	CLR_BIT(TIM2->CR1,GPTIM_UDIS);

	/*Disable Update Interrupt */
	CLR_BIT(TIM2->DIER,GPTIM_UIE);

	/*F(TIM2)=8M/(1+PSC)=1MHz
	*Ticks equal 1us*/
	TIM2->PSC=7;
}
/* Synchronous API  Lock Processor For Certain Ticks
   *Disable SysTick interrupt
   *enable Systick
   *polling flag (underflow)
   *Tick=>1us*/
void TIM2_voidSetBusyWait( u32 Copy_u32Ticks )
{
	u32 Local_u32Routine=Copy_u32Ticks/65535;
	u32 Local_u32Fraction=Copy_u32Ticks%65535;
	while(Local_u32Routine--)
	{
		TIM2->ARR=65535;
	    /*Re-Initialize the the counter & prescaller*/
	    SET_BIT(TIM2->EGR,GPTIM_UG);
		/*Enable counter*/
		SET_BIT(TIM2->CR1,GPTIM_CEN);
		while(GET_BIT(TIM2->SR,GPTIM_UIF)==0);
		/*Clear Uderflow Flag*/
		CLR_BIT(TIM2->SR,GPTIM_UIF);
	}

	TIM2->ARR=Local_u32Fraction;
	/*Re-Initialize the the counter & prescaller*/
	SET_BIT(TIM2->EGR,GPTIM_UG);
	/*Enable counter*/
	SET_BIT(TIM2->CR1,GPTIM_CEN);
	while(GET_BIT(TIM2->SR,GPTIM_UIF)==0);
	/*Clear Uderflow Flag*/
	CLR_BIT(TIM2->SR,GPTIM_UIF);
	/*Stop Timer*/
	CLR_BIT(TIM2->CR1,GPTIM_CEN);



}
void TIM2_voidSetIntervalSingle( u32 Copy_u32Ticks, void (*Copy_ptr)(void) )
{
	/*save CallBack*/
	TIM2_CallBack=Copy_ptr;
	/*Set Mode to Single*/
	TIM2_u8ModeOfInterval=SINGLE_INTERVAL;

	Global2_u32Routine =(Copy_u32Ticks/65535);
	Global2_u32Fraction=Copy_u32Ticks%65535;
	/*    to be initialize before enter the interrupt */
	Global2_u32CopyRoutine=Global2_u32Routine;
	/*stop timer */
	CLR_BIT(TIM2->CR1,GPTIM_CEN);

	if(Global2_u32Routine)
	{
		TIM2->ARR=65535;
	    /*Re-Initialize the the counter & prescaller*/
	    SET_BIT(TIM2->EGR,GPTIM_UG);
		/*Enable counter*/
		SET_BIT(TIM2->CR1,GPTIM_CEN);
	}
	else
	{
		TIM2->ARR=Global2_u32Fraction;
	    /*Re-Initialize the the counter & prescaller*/
	    SET_BIT(TIM2->EGR,GPTIM_UG);
		/*Enable counter*/
		SET_BIT(TIM2->CR1,GPTIM_CEN);
	}

	/*Enable Update Interrupt */
	SET_BIT(TIM2->DIER,GPTIM_UIE);
}
void TIM2_voidSetIntervalPeriodic( u32 Copy_u32Ticks, void (*Copy_ptr)(void) )
{
	/*save CallBack*/
	TIM2_CallBack=Copy_ptr;
	/*Set Mode to Periodic*/
	TIM2_u8ModeOfInterval=PERIODIC_INTERVAL;

	Global2_u32Routine =(Copy_u32Ticks/65535) ;
	Global2_u32Fraction=Copy_u32Ticks%65535;

	if(Global2_u32Routine)
	{
		TIM2->ARR=65535;
	    /*Re-Initialize the the counter & prescaller*/
	    SET_BIT(TIM2->EGR,GPTIM_UG);
		/*Enable counter*/
		SET_BIT(TIM2->CR1,GPTIM_CEN);
	}
	else
	{
		TIM2->ARR=Global2_u32Fraction;
	    /*Re-Initialize the the counter & prescaller*/
	    SET_BIT(TIM2->EGR,GPTIM_UG);
		/*Enable counter*/
		SET_BIT(TIM2->CR1,GPTIM_CEN);
	}

	/*Enable Update Interrupt */
	SET_BIT(TIM2->DIER,GPTIM_UIE);
}

void TIM2_voidStopInterval(void)
{
	//*Disable Update Interrupt
    CLR_BIT(TIM2->DIER,GPTIM_UIE);
	//*Stop Timer
	CLR_BIT(TIM2->CR1,GPTIM_CEN);
}
u32  TIM2_u16GetElapsedTime(void)
{
	u32 Local_u32ElapsedTime;
	u16 Local_u16Cycle=Global2_u32Routine - Global2_u32CopyRoutine;

	Local_u32ElapsedTime=((TIM2->ARR)-(TIM2->CNT))+(Local_u16Cycle*65535);
	return Local_u32ElapsedTime;
}
u32  TIM2_u16GetRemainingTime(void)
{
	u32 local_u32RemainingTime=0;
	if(Global2_u32CopyRoutine>=1)
	{
		local_u32RemainingTime=(TIM2->CNT)+(Global2_u32CopyRoutine-1)*65535+Global2_u32Fraction;
	}
	else if(Global2_u32CopyRoutine==0)
	{
		local_u32RemainingTime=TIM2->CNT;
	}
	return local_u32RemainingTime;
}

void TIM2_IRQHandler(void)
{
	if(Global2_u8Flag){
		Global2_u32CopyRoutine=Global2_u32Routine;
		Global2_u8Flag=0;
	}

	//Local_u32Routine=Global_u32Routine;
	if((Global2_u32CopyRoutine)>1)
	{
		Global2_u32CopyRoutine--;
		//Clear Uderflow Flag
		CLR_BIT(TIM2->SR,GPTIM_UIF);
		TIM2->ARR=65535;
	    //*Re-Initialize the the counter & prescaller
	    SET_BIT(TIM2->EGR,GPTIM_UG);

	}
	else if((Global2_u32CopyRoutine)==1)
	{
		Global2_u32CopyRoutine--;
		//*Clear Uderflow Flag
		CLR_BIT(TIM2->SR,GPTIM_UIF);
		TIM2->ARR=Global2_u32Fraction;
	    //*Re-Initialize the the counter & prescaller
	    SET_BIT(TIM2->EGR,GPTIM_UG);

	}
	else
	{
		if(TIM2_u8ModeOfInterval==SINGLE_INTERVAL)
		{
			//*Disable Update Interrupt
	        CLR_BIT(TIM2->DIER,GPTIM_UIE);
			//*Stop Timer
			CLR_BIT(TIM2->CR1,GPTIM_CEN);
		}
		//*Callback Notification
		TIM2_CallBack();
		//Clear Uderflow Flag/
		CLR_BIT(TIM2->SR,GPTIM_UIF);

		if(Global2_u32Routine)TIM2->ARR=65535;
		else TIM2->ARR=Global2_u32Fraction;
	    //*Re-Initialize the the counter & prescaller
	    SET_BIT(TIM2->EGR,GPTIM_UG);

		Global2_u8Flag=1;
	}

}
