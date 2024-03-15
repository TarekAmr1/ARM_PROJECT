/*
np * DAC_program.c
 *
 *  Created on: Jan 23, 2024
 *      Author: philo
 */

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "GPIO_interface.h"
#include "SYSTICK_interface.h"
#include "DAC_interface.h"



void HR2RDAC_vInit(void)
{
	MGPIO_vSetMode(PORT_B,PIN_7,MODE_OUTPUT);
	MGPIO_vSetMode(PORT_B,PIN_8,MODE_OUTPUT);
	MGPIO_vSetMode(PORT_B,PIN_9,MODE_OUTPUT);
	MGPIO_vSetMode(PORT_B,PIN_10,MODE_OUTPUT);
	MGPIO_vSetMode(PORT_B,PIN_12,MODE_OUTPUT);
	MGPIO_vSetMode(PORT_B,PIN_13,MODE_OUTPUT);
	MGPIO_vSetMode(PORT_B,PIN_14,MODE_OUTPUT);
	MGPIO_vSetMode(PORT_B,PIN_15,MODE_OUTPUT);

	MGPIO_vSetOutputMode(PORT_B,PIN_7,OUTPUT_PUSHPULL,SPEED_MEDUIEM);
	MGPIO_vSetOutputMode(PORT_B,PIN_8,OUTPUT_PUSHPULL,SPEED_MEDUIEM);
	MGPIO_vSetOutputMode(PORT_B,PIN_9,OUTPUT_PUSHPULL,SPEED_MEDUIEM);
	MGPIO_vSetOutputMode(PORT_B,PIN_10,OUTPUT_PUSHPULL,SPEED_MEDUIEM);
	MGPIO_vSetOutputMode(PORT_B,PIN_12,OUTPUT_PUSHPULL,SPEED_MEDUIEM);
	MGPIO_vSetOutputMode(PORT_B,PIN_13,OUTPUT_PUSHPULL,SPEED_MEDUIEM);
	MGPIO_vSetOutputMode(PORT_B,PIN_14,OUTPUT_PUSHPULL,SPEED_MEDUIEM);
	MGPIO_vSetOutputMode(PORT_B,PIN_15,OUTPUT_PUSHPULL,SPEED_MEDUIEM);

}
void HR2RDAC_vSetData(const u8 *arr,u32 arrsize,u8 *CopyFlage)
{

	u32 L_counter;
	for (L_counter=0;L_counter<arrsize;L_counter++)
	{
		if (*CopyFlage == 0)
		{
			MGPIO_vWritePin(PORT_B,PIN_7,GET_BIT(arr[L_counter],PIN_7));
			MGPIO_vWritePin(PORT_B,PIN_8,GET_BIT(arr[L_counter],PIN_8));
			MGPIO_vWritePin(PORT_B,PIN_9,GET_BIT(arr[L_counter],PIN_9));
			MGPIO_vWritePin(PORT_B,PIN_10,GET_BIT(arr[L_counter],PIN_10));
			MGPIO_vWritePin(PORT_B,PIN_12,GET_BIT(arr[L_counter],PIN_12));
			MGPIO_vWritePin(PORT_B,PIN_13,GET_BIT(arr[L_counter],PIN_13));
			MGPIO_vWritePin(PORT_B,PIN_14,GET_BIT(arr[L_counter],PIN_14));
			MGPIO_vWritePin(PORT_B,PIN_15,GET_BIT(arr[L_counter],PIN_15));
			MSYSYTICK_voidDelayUs(125);
			MSYSYTICK_voidCtrlIntState(SYSTICKEnable);

		}
		else
		{
			break;


		}
	}
}




