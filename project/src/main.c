#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "MRCC_interface.h"
#include "NVIC_interface.h"
#include "SYSTICK_interface.h"
#include "EXTI_interface.h"
#include "GPIO_interface.h"
#include "SPI_interface.h"
#include "TFT_interface.h"
#include "DAC_interface.h"
#include "STP_interface.h"
#include "Timer2_interface.h"
#include "Timer3_interface.h"

#include "img.h"
#include "MessiImage.h"
#include "song.h"
#include "song2.h"






u8 case1_flage=0;

u8 case2_flage=0;
void ledmat_M(void);
void ledmat_S(void);
void EXTI1_Func()
{
	case2_flage=1;
	case1_flage=1;

}
void EXTI0_Func()
{
	case2_flage=0;
	case1_flage=1;

}
int main (void)
{

	MRCC_vInt();

	MRCC_vEnable(RCC_AHB1,AHB1_GPIOA);
	MRCC_vEnable(RCC_AHB1,AHB1_GPIOB);
	MRCC_vEnable(RCC_APB2, APB2_SYSCFG);
	MRCC_vEnable(RCC_APB2,APB2_SPI1);
	MRCC_vEnable(RCC_APB1,APB1_TIM2);
	MRCC_vEnable(RCC_APB1,APB1_TIM3);

	//********************//
	MGPIO_vSetMode(PORT_A,PIN_0,MODE_OUTPUT);
	MGPIO_vSetMode(PORT_A,PIN_1,MODE_OUTPUT);
	MGPIO_vSetOutputMode(PORT_A,PIN_0,OUTPUT_PUSHPULL,SPEED_LOW);
	MGPIO_vSetOutputMode(PORT_A,PIN_1,OUTPUT_PUSHPULL,SPEED_LOW);
	//**************************//
	MGPIO_vSetMode(PORT_B, PIN_0, MODE_INPUT);
	MGPIO_vSetInputMode(PORT_B, PIN_0, INPUT_PULLUP);
	//**************************//
	MGPIO_vSetMode(PORT_B, PIN_1, MODE_INPUT);
	MGPIO_vSetInputMode(PORT_B, PIN_1, INPUT_PULLUP);
	//**************************//
	MGPIO_vSetMode(PORT_B, PIN_2, MODE_INPUT);
	MGPIO_vSetInputMode(PORT_B, PIN_2, INPUT_PULLUP);
	//*********************************//
	MGPIO_vSetMode(PORT_A,PIN_4,MODE_ALTERNATIVE);
	MGPIO_vSetMode(PORT_A,PIN_5,MODE_ALTERNATIVE);
	MGPIO_vSetMode(PORT_A,PIN_6,MODE_ALTERNATIVE);
	MGPIO_vSetMode(PORT_A,PIN_7,MODE_ALTERNATIVE);
	MGPIO_vSetAlternativeMode(PORT_A,PIN_4,ALTR_SPI);
	MGPIO_vSetAlternativeMode(PORT_A,PIN_5,ALTR_SPI);
	MGPIO_vSetAlternativeMode(PORT_A,PIN_6,ALTR_SPI);
	MGPIO_vSetAlternativeMode(PORT_A,PIN_7,ALTR_SPI);
	//*******************************************//
	MGPIO_vSetMode(PORT_A,PIN_0,MODE_OUTPUT);
	MGPIO_vSetMode(PORT_A,PIN_1,MODE_OUTPUT);
	//******************************************************/
	MGPIO_vSetOutputMode(PORT_A,PIN_0,OUTPUT_PUSHPULL,SPEED_LOW);
	MGPIO_vSetOutputMode(PORT_A,PIN_1,OUTPUT_PUSHPULL,SPEED_LOW);
	//*******************************************//
	HR2RDAC_vInit();
	//*************************************//
	S2P_vInit();

	MGPIO_vSetMode(PORT_A,PIN_8,MODE_OUTPUT);
	MGPIO_vSetOutputMode(PORT_A,PIN_8,OUTPUT_PUSHPULL,SPEED_LOW);

	MGPIO_vSetMode(PORT_A,PIN_9,MODE_OUTPUT);
	MGPIO_vSetOutputMode(PORT_A,PIN_9,OUTPUT_PUSHPULL,SPEED_LOW);

	MGPIO_vSetMode(PORT_A,PIN_10,MODE_OUTPUT);
	MGPIO_vSetOutputMode(PORT_A,PIN_10,OUTPUT_PUSHPULL,SPEED_LOW);
	//*************************************//
	SPI1_voidInit();
	//********************************//
	MSYSYTICK_voidInit();
	TIM2_voidInit();
	TIM3_voidInit();
	TFT_voidvInit();
	MSYSYTICK_voidCtrlIntState(SYSTICKEnable);
	//***************************//
	MEXTI_voidInerruptSetPort(EXTI_LineOne,PORT_B);
	MEXTI_voidInterruptTrigger(EXTI_LineOne,EXTI_Falling);
	MEXTI_voidInterruptEnableDisable(EXTI_LineOne,EXTI_Enable);
	MEXTI1_voidCallBack(EXTI1_Func);
	//***************************//
	MEXTI_voidInerruptSetPort(EXTI_LineZero,PORT_B);
	MEXTI_voidInterruptTrigger(EXTI_LineZero,EXTI_Falling);
	MEXTI_voidInterruptEnableDisable(EXTI_LineZero,EXTI_Enable);
	MEXTI0_voidCallBack(EXTI0_Func);
	//**********************//
	NVIC_VoidSetInterruptGroupMode(Group16Sub0);
	NVIC_VoidSetInterruptPriority(6,1,0);
	NVIC_vEnable(6);
	NVIC_vEnable(7);
	//****************************//


	while (1)
	{




		switch(case2_flage)

		{
		case 0 :
			case1_flage=0;
			TFT_voidDisplay(img);
			for(int i=0;i<2500;i++)
			{
				if(case1_flage==0){
					ledmat_M();
				}
				else{
					break;
				}
			}

			HR2RDAC_vSetData(song_raw2,47936, &case1_flage);
			MSYSYTICK_voidCtrlIntState(SYSTICKEnable);
			break;
		case 1:
			case1_flage=0;
			TFT_voidDisplay(MessiImage);
			for(int i=0;i<2500;i++)
			{
				if(case1_flage==0){
					ledmat_S();
				}
				else{
					break;
				}
			}

			HR2RDAC_vSetData(song_raw,47472,&case1_flage);
			MSYSYTICK_voidCtrlIntState(SYSTICKEnable);
			break;
		default:
			asm("NOP");
			break;

		}
	}

}

void ledmat_M(void)
{
	S2P_vSendData(0xFEFC);
	TIM2_voidSetBusyWait(1);
	S2P_vSendData(0xFD04);
	TIM2_voidSetBusyWait(1);
	S2P_vSendData(0xFB08);
	TIM2_voidSetBusyWait(1);
	S2P_vSendData(0xF710);
	TIM2_voidSetBusyWait(1);
	S2P_vSendData(0xEF08);
	TIM2_voidSetBusyWait(1);
	S2P_vSendData(0xDF04);
	TIM2_voidSetBusyWait(1);
	S2P_vSendData(0xBFFC);
	TIM2_voidSetBusyWait(1);




}

void ledmat_S(void)
{
	S2P_vSendData(0xFE00);
	TIM2_voidSetBusyWait(1);
	S2P_vSendData(0xFD9E);
	TIM2_voidSetBusyWait(1);
	S2P_vSendData(0xFB92);
	TIM2_voidSetBusyWait(1);
	S2P_vSendData(0xF792);
	TIM2_voidSetBusyWait(1);
	S2P_vSendData(0xEF92);
	TIM2_voidSetBusyWait(1);
	S2P_vSendData(0xDF92);
	TIM2_voidSetBusyWait(1);
	S2P_vSendData(0xBFF2);
	TIM2_voidSetBusyWait(1);
}


