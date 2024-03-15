/*
 * Timer3_program.c
 *
 *  Created on: Mar 10, 2024
 *      Author: philo
 */
#include"STD_TYPES.h"
#include"BIT_MATH.h"
#include"Timer3_private.h"
#include"Timer3_config.h"
#include"Timer3_interface.h"

void TIM3_voidInit(void)
{
	/*Up Counter Mode*/
	TIM3->CR1&=~(0b111<<4);
	TIM3->CR1|=(DOWNCOUNTER<<4);

	/*ARR is Buffered can't changed Execpt at Update Event*/
	SET_BIT(TIM3->CR1,GPTIM_ARPE);

	/*Only Counter OverFlow UnderFlow Generate update Interrupt */
	SET_BIT(TIM3->CR1,GPTIM_URS);

	/*Enable Update Event */
	CLR_BIT(TIM3->CR1,GPTIM_UDIS);

	/*Disable Update Interrupt */
	CLR_BIT(TIM3->DIER,GPTIM_UIE);

	/*F(TIM2)=8M/(1+PSC)=1MHz
	*Ticks equal 1us*/
	TIM3->PSC=7;
}
/* Synchronous API  Lock Processor For Certain Ticks
   *Disable SysTick interrupt
   *enable Systick
   *polling flag (underflow)
   *Tick=>1us*/
void TIM3_voidSetBusyWait( u32 Copy_u32Ticks )
{
	u32 Local_u32Routine=Copy_u32Ticks/65535;
	u32 Local_u32Fraction=Copy_u32Ticks%65535;
	while(Local_u32Routine--)
	{
		TIM3->ARR=65535;
	    /*Re-Initialize the the counter & prescaller*/
	    SET_BIT(TIM3->EGR,GPTIM_UG);
		/*Enable counter*/
		SET_BIT(TIM3->CR1,GPTIM_CEN);
		while(GET_BIT(TIM3->SR,GPTIM_UIF)==0);
		/*Clear Underflow Flag*/
		CLR_BIT(TIM3->SR,GPTIM_UIF);
	}

	TIM3->ARR=Local_u32Fraction;
	/*Re-Initialize the the counter & prescaller*/
	SET_BIT(TIM3->EGR,GPTIM_UG);
	/*Enable counter*/
	SET_BIT(TIM3->CR1,GPTIM_CEN);
	while(GET_BIT(TIM3->SR,GPTIM_UIF)==0);
	/*Clear Uderflow Flag*/
	CLR_BIT(TIM3->SR,GPTIM_UIF);
	/*Stop Timer*/
	CLR_BIT(TIM3->CR1,GPTIM_CEN);



}
void TIM3_voidSetIntervalSingle( u32 Copy_u32Ticks, void (*Copy_ptr)(void) )
{
	/*save CallBack*/
	TIM3_CallBack=Copy_ptr;
	/*Set Mode to Single*/
	TIM3_u8ModeOfInterval=SINGLE_INTERVAL;

	Global3_u32Routine =(Copy_u32Ticks/65535);
	Global3_u32Fraction=Copy_u32Ticks%65535;
	/*    to be initialize before enter the interrupt */
	Global3_u32CopyRoutine=Global3_u32Routine;
	/*stop timer */
	CLR_BIT(TIM3->CR1,GPTIM_CEN);

	if(Global3_u32Routine)
	{
		TIM3->ARR=65535;
	    /*Re-Initialize the the counter & prescaller*/
	    SET_BIT(TIM3->EGR,GPTIM_UG);
		/*Enable counter*/
		SET_BIT(TIM3->CR1,GPTIM_CEN);
	}
	else
	{
		TIM3->ARR=Global3_u32Fraction;
	    /*Re-Initialize the the counter & prescaller*/
	    SET_BIT(TIM3->EGR,GPTIM_UG);
		/*Enable counter*/
		SET_BIT(TIM3->CR1,GPTIM_CEN);
	}

	/*Enable Update Interrupt */
	SET_BIT(TIM3->DIER,GPTIM_UIE);
}
void TIM3_voidSetIntervalPeriodic( u32 Copy_u32Ticks, void (*Copy_ptr)(void) )
{
	/*save CallBack*/
	TIM3_CallBack=Copy_ptr;
	/*Set Mode to Periodic*/
	TIM3_u8ModeOfInterval=PERIODIC_INTERVAL;

	Global3_u32Routine =(Copy_u32Ticks/65535) ;
	Global3_u32Fraction=Copy_u32Ticks%65535;

	if(Global3_u32Routine)
	{
		TIM3->ARR=65535;
	    /*Re-Initialize the the counter & prescaller*/
	    SET_BIT(TIM3->EGR,GPTIM_UG);
		/*Enable counter*/
		SET_BIT(TIM3->CR1,GPTIM_CEN);
	}
	else
	{
		TIM3->ARR=Global3_u32Fraction;
	    /*Re-Initialize the the counter & prescaller*/
	    SET_BIT(TIM3->EGR,GPTIM_UG);
		/*Enable counter*/
		SET_BIT(TIM3->CR1,GPTIM_CEN);
	}

	/*Enable Update Interrupt */
	SET_BIT(TIM3->DIER,GPTIM_UIE);
}

void TIM3_voidStopInterval(void)
{
	//*Disable Update Interrupt
    CLR_BIT(TIM3->DIER,GPTIM_UIE);
	//*Stop Timer
	CLR_BIT(TIM3->CR1,GPTIM_CEN);
}
u32  TIM3_u16GetElapsedTime(void)
{
	u32 Local_u32ElapsedTime;
	u16 Local_u16Cycle=Global3_u32Routine - Global3_u32CopyRoutine;

	Local_u32ElapsedTime=((TIM3->ARR)-(TIM3->CNT))+(Local_u16Cycle*65535);
	return Local_u32ElapsedTime;
}
u32  TIM3_u16GetRemainingTime(void)
{
	u32 local_u32RemainingTime=0;
	if(Global3_u32CopyRoutine>=1)
	{
		local_u32RemainingTime=(TIM3->CNT)+(Global3_u32CopyRoutine-1)*65535+Global3_u32Fraction;
	}
	else if(Global3_u32CopyRoutine==0)
	{
		local_u32RemainingTime=TIM3->CNT;
	}
	return local_u32RemainingTime;
}


void TIM3_IRQHandler(void)
{
	if(Global3_u8Flag){
		Global3_u32CopyRoutine=Global3_u32Routine;
		Global3_u8Flag=0;
	}
	if((Global3_u32CopyRoutine)>1)
	{
		Global3_u32CopyRoutine--;
		//Clear Uderflow Flag
		CLR_BIT(TIM3->SR,GPTIM_UIF);
		TIM3->ARR=65535;
	    //*Re-Initialize the the counter & prescaller
	    SET_BIT(TIM3->EGR,GPTIM_UG);

	}
	else if((Global3_u32CopyRoutine)==1)
	{
		Global3_u32CopyRoutine--;
		//*Clear Uderflow Flag
		CLR_BIT(TIM3->SR,GPTIM_UIF);
		TIM3->ARR=Global3_u32Fraction;
	    //*Re-Initialize the the counter & prescaller
	    SET_BIT(TIM3->EGR,GPTIM_UG);

	}
	else
	{
		if(TIM3_u8ModeOfInterval==SINGLE_INTERVAL)
		{
			//*Disable Update Interrupt
	        CLR_BIT(TIM3->DIER,GPTIM_UIE);
			//*Stop Timer
			CLR_BIT(TIM3->CR1,GPTIM_CEN);
		}
		//*Callback Notification
		TIM3_CallBack();
		//Clear Uderflow Flag/
		CLR_BIT(TIM3->SR,GPTIM_UIF);

		if(Global3_u32Routine)TIM3->ARR=65535;
		else TIM3->ARR=Global3_u32Fraction;
	    //*Re-Initialize the the counter & prescaller
	    SET_BIT(TIM3->EGR,GPTIM_UG);

		Global3_u8Flag=1;
	}

}






