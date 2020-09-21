#include "LcdLibrary.h"



bool LCD_Init(bool twoLineMode, bool largeFontMode)
{
	LCD_IO_CMD_DD = OUTPUT_MODE;  // configure i/o port 1 for output
	LCD_IO_CMD_PORT = INPUT_MODE; // clear i/o port 1
	LCD_IO_DATA_DD = OUTPUT_MODE; // configure i/o port 2 for output
	LCD_IO_DATA_PORT = INPUT_MODE; // clear i/o port 1
	return true;
}


void LCD_Wait()
{
	// Retain the command port(port 1) values as it is
	// set data port (port 2) to input mode
	LCD_IO_CMD_PORT &= 0b11111110; 
	LCD_IO_CMD_PORT |= (1 << LCD_RW); // set rw to 1 , for reading 
	
	LCD_IO_DATA_DD = INPUT_MODE;
	unsigned dataBus_val = (1 << LCD_BF);
		//check if the LCD is busy
		// read DB7, BF (busy flag) of LCD
	while (dataBus_val & (1 << LCD_BF))
	{
		LCD_Enable();
		dataBus_val = LCD_IO_DATA_PIN;
		LCD_Disable();
	}
}

void LCD_Enable()
{
	LCD_IO_CMD_PORT |= (1 << LCD_EN);	
}


void LCD_Disable()
{
	LCD_IO_CMD_PORT &= ~ (1 << LCD_EN);
}


void LCD_Write_Command(unsigned char commandValue)
{
	LCD_Wait();	
}

void LCD_Write_Data(unsigned char dataValue)
{
	LCD_Wait();
	LCD_IO_CMD_DD = OUTPUT_MODE;
	LCD_IO_CMD_PORT |= (1 << LCD_RS);
	LCD_IO_CMD_PORT &= ~(1 << LCD_RW);
	LCD_Enable();
	LCD_IO_DATA_PORT = dataValue;
	LCD_Disable();
}

void LCD_Write_String(char text[])
{
	unsigned idx = 0;
	while (text[idx] != '\0')
	{
		LCD_Write_Data(text[idx]);
		idx++;
	}
}