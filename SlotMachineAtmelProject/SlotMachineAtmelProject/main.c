/*
 * SlotMachineAtmelProject.c
 *
 * Created: 21/09/2020 16:06:22
 * Author : 230712
 */ 
#include "LcdLibrary.h"
#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
	LCD_Init(true,true);
	LCD_Write_String("here it is");
    while (1) 
    {
		
    }
}

