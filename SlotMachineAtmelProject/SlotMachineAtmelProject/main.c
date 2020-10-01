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
    /* Replace with your application code */

	LCD_Init(true,false);
	LCD_ShiftDisplay(false,false);
	LCD_Display_ON_OFF(true,false,false);
	LCD_Clear();
	LCD_Home();
	LCD_Write_String("Biplav");
#if 0	
	unsigned char data = 0x23; // start with 0,1,2
	LCD_SetCursorPosition(0x00,1);
	unsigned char WheelValues[] = { DOLLAR_SYMBOL,'0','1',YEN_SYMBOL,'3','4',PI_SYMBOL,HASH_SYMBOL,'5','6',SUMMATION_SYMBOL,'7','1','0','9','8','3','8','4','5','6','9'};
	for(int i=0;i<16;i++)
	{
		LCD_Write_Data(WheelValues[i]);
		data++;
		_delay_ms(1000);
	}
#endif	
	LCD_Clear();
	SM_Init();
	_delay_ms(5000);
    while (1) 
    {
		SM_SpinWheel();
    }
}

