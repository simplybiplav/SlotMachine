#include "LcdLibrary.h"

bool gTwoLineMode = false; 

bool LCD_Init(bool twoLineMode, bool largeFontMode)
{
    LCD_Set_CMD_Port_Out(ALL_BITS);
    LCD_Write_CMD_Port(ALL_BITS, false); //clear bits
	LCD_IO_DATA_DD = OUTPUT_MODE; // configure i/o port 2 for output
	LCD_IO_DATA_PORT = CLEAR_BITS; // clear i/o port 1

    unsigned char Command_value = FUNCTION_CMD;   
    if(true == twoLineMode)                                                           
    {                                                                              
        gTwoLineMode = true;
        Command_value |= (1 << FUNCTION_CMD_LINE_BIT);    
    }                                                                              
    else                                                                           
    {   // One-line mode                                                           
        if(true == largeFontMode)                                                     
        {
            Command_value |= (1 <<FUNCTION_CMD_FONT_BIT);   
        }                                                                          
    }                                                                              
                                                                                   
    LCD_Write_Command (Command_value);


	return true;
}


void LCD_Set_CMD_Port_Out(unsigned bitsToWrite)
{
    if (bitsToWrite == ALL_BITS)
    {
        LCD_IO_CMD_DD |= (1 << LCD_IO_CMD_PORT_RS ) | (1 << LCD_IO_CMD_PORT_RW) | ( 1 << LCD_IO_CMD_PORT_EN); 
    }
    else
    {
        // ensure other bits are not changed
        LCD_IO_CMD_DD |= bitsToWrite & ( (1 << LCD_IO_CMD_PORT_RS ) | (1 << LCD_IO_CMD_PORT_RW) | ( 1 << LCD_IO_CMD_PORT_EN) );
    }
}

void LCD_Set_CMD_Port_In(unsigned bitsToWrite)
{

    if (bitsToWrite == ALL_BITS)
    {
        LCD_IO_CMD_DD &= ~ ( (1 << LCD_IO_CMD_PORT_RS ) | (1 << LCD_IO_CMD_PORT_RW) | ( 1 << LCD_IO_CMD_PORT_EN) ) ; 
    }
    else
    {
        // ensure other bits are not changed
        LCD_IO_CMD_DD &= ~ (bitsToWrite & ( (1 << LCD_IO_CMD_PORT_RS ) | (1 << LCD_IO_CMD_PORT_RW) | ( 1 << LCD_IO_CMD_PORT_EN) ) );
    }

}

unsigned LCD_Read_CMD_Port(unsigned bitsToBeRead )
{
	LCD_Wait();	
    unsigned readVal = 0;
    LCD_Set_CMD_Port_In(bitsToBeRead);
    readVal = LCD_IO_CMD_PIN & ( (1 << LCD_IO_CMD_PORT_RS ) | (1 << LCD_IO_CMD_PORT_RW) | ( 1 << LCD_IO_CMD_PORT_EN) );
    return readVal;

}

void LCD_Write_CMD_Port(unsigned bitsToWrite, bool setReset)
{
    if (setReset)
    {
        LCD_IO_CMD_PORT |=  bitsToWrite & ( (1 << LCD_IO_CMD_PORT_RS ) | (1 << LCD_IO_CMD_PORT_RW) | ( 1 << LCD_IO_CMD_PORT_EN) );
    }
    else
    {

        LCD_IO_CMD_PORT &= ~ (bitsToWrite & ( (1 << LCD_IO_CMD_PORT_RS ) | (1 << LCD_IO_CMD_PORT_RW) | ( 1 << LCD_IO_CMD_PORT_EN) ));
    }
}

void LCD_Wait()
{
	// Retain the command port(port 1) values as it is
	// set data port (port 2) to input mode
    LCD_Set_CMD_Port_In( (1 << LCD_IO_CMD_PORT_RS) );
    LCD_Write_CMD_Port( (1<< LCD_IO_CMD_PORT_RW) ,true);
	
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
    LCD_Write_CMD_Port( (1<< LCD_IO_CMD_PORT_EN), true);
}


void LCD_Disable()
{
    LCD_Write_CMD_Port( (1<< LCD_IO_CMD_PORT_EN), false);
}


void LCD_Write_Command(unsigned char commandValue)
{
	LCD_Wait();	
    LCD_Set_CMD_Port_Out(ALL_BITS);
    LCD_Write_CMD_Port( (1 << LCD_IO_CMD_PORT_RS) , false);
    LCD_Write_CMD_Port( (1 << LCD_IO_CMD_PORT_RW) , false);
	LCD_Enable();
    LCD_IO_DATA_DD = OUTPUT_MODE;
	LCD_IO_DATA_PORT = commandValue;
	LCD_Disable();
}

void LCD_Write_Data(unsigned char dataValue)
{
	LCD_Wait();
    LCD_Set_CMD_Port_Out(ALL_BITS);
    LCD_Write_CMD_Port( (1 << LCD_IO_CMD_PORT_RS) , true);
    LCD_Write_CMD_Port( (1 << LCD_IO_CMD_PORT_RW) , false);
	LCD_Enable();
    LCD_IO_DATA_DD = OUTPUT_MODE;
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

void LCD_Clear()			// Clear the LCD display
{
	LCD_Write_Command (CLEAR_CMD);
	_delay_ms(2);	// takes 1.5 ms to complete, so wait 
}

void LCD_Home()			// Set the cursor to the 'home' position
{
	LCD_Write_Command (HOME_CMD);
	_delay_ms(2);	// takes 1.5 ms to complete, so wait 
}

void LCD_Display_ON_OFF(bool displayON, bool cursorON, bool cursorPositionON)
{
    unsigned commandValue = DISPLAY_CMD;
    if (displayON) commandValue |= (1 << DISPLAY_CMD_DISPLAY_ON_BIT);
    if (cursorON) commandValue |= (1 << DISPLAY_CMD_CURSOR_ON_BIT);
    if (cursorPositionON) commandValue |= (1 << DISPLAY_CMD_CUR_POS_ON_BIT);

    LCD_Write_Command(commandValue);
} 



void LCD_ShiftDisplay(bool shiftDisplayON , bool directionRight )
{	
    unsigned commandValue = DISPLAY_SHIFT_CMD;
    if (shiftDisplayON) commandValue |= (1<< DISPLAY_SHIFT_CMD_SHIFT_CONTROL_BIT);
    if(directionRight) commandValue |= (1 <<DISPLAY_SHIFT_CMD_DIRECTION_BIT);
    LCD_Write_Command(commandValue);
}

void LCD_SetCursorPosition(unsigned char columnPosition /*0 - 40 */, unsigned char rowPosition /*0 for top row, 1 for bottom row*/)
{
// Function Set 001(DL)NFXX
//DL = interface data 8/4 bits
// N = Number of Line
// 00H to 4FH in one line mode
// 00H to 27H in 1st line , 40H to 67H in 2nd line
// F = Font Size 5*11 / 5*8
//set DGRAM address 0b1(AC6-AC0)
    if (true == gTwoLineMode)
    {
        LCD_Write_Command(DGRAM_CMD | (40 * rowPosition + columnPosition )); 
    }
    else
    {
        LCD_Write_Command(DGRAM_CMD | columnPosition ); 
    }
}
