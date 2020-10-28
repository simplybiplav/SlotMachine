/*
 * SlotMachineAtmelProject.c
 *
 * Created: 21/09/2020 16:06:22
 * Author : 230712
 */ 
#include "LcdLibrary.h"
#include "SlotMachine.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

int main(void)
{

	LCD_Init(true,false);
	LCD_ShiftDisplay(false,false);
	LCD_Display_ON_OFF(true,false,false);
	LCD_Clear();
	LCD_Home();
	SM_Init();
	sei();
    while (1) 
    {
		SM_SpinWheel();
    }
}

ISR(INT0_vect) // Interrupt Handler for H/W INT 0
{
	if(gGameData.smState == SM_IDLE)
	{
		gGameData.smState = SM_USER_WAIT;
		LCD_Clear();
		SM_UpdateLCD();
		_delay_ms(KEY_DEBOUNCE_WAIT);
		EIFR |= 0b00000011;
		return;
	}
	KP_Disable_Spin();
	//KP_Disable_Bet();
	SM_ToggleSpin();
	SM_SpinPressedLights();
	_delay_ms(KEY_DEBOUNCE_WAIT);		// Short delay to debounce the push-button
	SM_UserWaitLights();
	EIFR |= 0b00000011;
	idleTimeOut = 0;
	//KP_Enable_Spin();
	//KP_Enable_Bet();
}



ISR(INT1_vect) // Interrupt Handler for H/W INT 0
{
	if(gGameData.smState == SM_IDLE)
	{
		gGameData.smState = SM_USER_WAIT;
		LCD_Clear();
		SM_UpdateLCD();
		_delay_ms(KEY_DEBOUNCE_WAIT);
		EIFR |= 0b00000011;
		return;
	}
	KP_Disable_Bet();
	KP_Disable_Spin();
	SM_IncreaseBet();
	SM_UpdateLCDPlayerBet();
	SM_BetPressedLights();
	_delay_ms(KEY_DEBOUNCE_WAIT);		// Short delay to debounce the push-button
	SM_UserWaitLights();
	EIFR |= 0b00000011;
	idleTimeOut = 0;
	KP_Enable_Bet();
	KP_Enable_Spin();
}



ISR(TIMER1_COMPA_vect) // TIMER1_CompareA_Handler (Interrupt Handler for Timer 1)
{
	idleTimeOut++; // Increment the number of elapsed seconds while the timer has been running
	if (idleTimeOut >= IDLE_TIME_OUT_VALUE)
	{
		idleTimeOut = 0;
		SM_DisableIdleTimer();
		SM_Idle();
	}
}
