#ifndef _LCD_LIBRARY_H
#define _LCD_LIBRARY_H


/* 


This library is for QC1602A LCD
pin configuration:
1 : vss
2 : vdd
3 : v0
4 : RS
5 : R/W
6 : E
7-14 : D0-D7 (data bus )
15 : A (back light 5 v)
16 : K  (back light 0 v)


PORT 1(0->5) will be connected to 1->6
PORT 2(0->7) will be connected to 7->14
*/
#include <stdbool.h>
#include "config.h"
#include <avr/io.h>

#if !defined(LCD_IO_CMD_PORT_LETTER) || !defined(LCD_IO_DATA_PORT_LETTER)
#error "LCD_IO_CMD_PORT_LETTER and LCD_IO_DATA_PORT_LETTER not defined"
#endif

#define HASH_SYMBOL 0x23
#define DOLLAR_SYMBOL 0x24 
#define SUMMATION_SYMBOL 0xF7
#define PI_SYMBOL 0xF8
#define YEN_SYMBOL 0x55

// RS = 0 and RW= 0 for command
#define CLEAR_CMD     0b00000001 //write time 1.52 ms  
#define HOME_CMD      0b00000010 //write time 1.52 ms



// Display ON/OFF     0b00001DCB 
// D =1, Display ON
// C=1, Cursor ON
// B=1, Cursor Position On
#define DISPLAY_CMD 0b00001000    //write time 37 us
#define DISPLAY_CMD_DISPLAY_ON_BIT 2 
#define DISPLAY_CMD_CURSOR_ON_BIT 1 
#define DISPLAY_CMD_CUR_POS_ON_BIT 0 


//Cursor or Display Shift 0b0001(S/C)(R/L)XX
// set cursor shift without changing DDRAM data 
#define DISPLAY_SHIFT_CMD 0b00010000
#define DISPLAY_SHIFT_CMD_SHIFT_CONTROL_BIT 3
#define DISPLAY_SHIFT_CMD_DIRECTION_BIT 2


// Function Set 001(DL)NFXX
//DL = interface data 8/4 bits
// N = Number of Line
// 00H to 4FH in one line mode
// 00H to 27H in 1st line , 40H to 67H in 2nd line
// F = Font Size 5*11 / 5*8

#define FUNCTION_CMD 0b00100000
#define FUNCTION_CMD_INTERFACE_BIT 4
#define FUNCTION_CMD_LINE_BIT 3
#define FUNCTION_CMD_FONT_BIT 2



// set CGRAM address 0b01(AC5-AC0)
#define CGRAM_CMD 0b01000000 

//set DGRAM address 0b1(AC6-AC0)

#define DGRAM_CMD 0b10000000 




/*
For read RS = 1, RW=1
For Write RS = 1, RW=0
For reading busy flag RS=0,RW = 1
*/

#define LCD_D0 0
#define LCD_D1 1
#define LCD_D2 2
#define LCD_D3 3
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7

#define LCD_BF LCD_D7


#define LCD_ROW_1 0
#define LCD_ROW_2 1

#define LCD_IO_CMD_DD  PORT_LETTER_TO_DD(LCD_IO_CMD_PORT_LETTER)
#define LCD_IO_CMD_PORT  PORT_LETTER_TO_PORT(LCD_IO_CMD_PORT_LETTER)
#define LCD_IO_CMD_PIN  PORT_LETTER_TO_PIN(LCD_IO_CMD_PORT_LETTER)
#define LCD_IO_DATA_DD  PORT_LETTER_TO_DD(LCD_IO_DATA_PORT_LETTER)
#define LCD_IO_DATA_PORT PORT_LETTER_TO_PORT(LCD_IO_DATA_PORT_LETTER)
#define LCD_IO_DATA_PIN  PORT_LETTER_TO_PIN(LCD_IO_DATA_PORT_LETTER)

extern bool gTwoLineMode;
/*
#define LCD_IO_1_DD  _LCD_IO_1_DD(LCD_IO_1ST_PORT_LETTER)
#define _LCD_IO_1_DD(PORT_LETTER)  __LCD_IO_1_DD(PORT_LETTER)
#define __LCD_IO_1_DD(PORT_LETTER)  DDR ## PORT_LETTER

#define LCD_IO_1_PORT  _LCD_IO_1_PORT(LCD_IO_1ST_PORT_LETTER)
#define _LCD_IO_1_PORT(PORT_LETTER)  __LCD_IO_1_PORT(PORT_LETTER)
#define __LCD_IO_1_PORT(PORT_LETTER)  PORT ## PORT_LETTER

#define LCD_IO_1_PIN  _LCD_IO_1_PIN(LCD_IO_1ST_PORT_LETTER)
#define _LCD_IO_1_PIN(PORT_LETTER)  __LCD_IO_1_PIN(PORT_LETTER)
#define __LCD_IO_1_PIN(PORT_LETTER)  PIN ## PORT_LETTER

#define LCD_IO_2_DD  _LCD_IO_2_DD(LCD_IO_2ND_PORT_LETTER)
#define _LCD_IO_2_DD(PORT_LETTER)  __LCD_IO_2_DD(PORT_LETTER)
#define __LCD_IO_2_DD(PORT_LETTER)  DDR ## PORT_LETTER

#define LCD_IO_2_PORT  _LCD_IO_2_PORT(LCD_IO_2ND_PORT_LETTER)
#define _LCD_IO_2_PORT(PORT_LETTER)  __LCD_IO_2_PORT(PORT_LETTER)
#define __LCD_IO_2_PORT(PORT_LETTER)  PORT ## PORT_LETTER

#define LCD_IO_2_PIN  _LCD_IO_2_PIN(LCD_IO_2ND_PORT_LETTER)
#define _LCD_IO_2_PIN(PORT_LETTER)  __LCD_IO_2_PIN(PORT_LETTER)
#define __LCD_IO_2_PIN(PORT_LETTER)  PIN ## PORT_LETTER

*/

//#include <string.h>

/*****************************************************************************

bool LCD_Init(bool twoLineMode, bool largeFontMode);
Arguments : 2
Inputs : 2
 twoLineMode              :=  1(false) or 2(true) line mode
 largeFontMode            :=  (false) = 5*8 pixels, (true) = 5*11 pixels
Output : None
Return : bool             := success (true) and failure (false)

*****************************************************************************/
bool LCD_Init(bool twoLineMode ,
                bool largeFontMode);


void LCD_Set_CMD_Port_Out(unsigned bitsToWrite = ALL_BITS);
void LCD_Set_CMD_Port_In(unsigned bitsToWrite = ALL_BITS);
unsigned LCD_Read_CMD_Port(unsigned bitsToBeRead = ALL_BITS);
void LCD_Write_CMD_Port(unsigned bitsToWrite,bool setReset);
void LCD_Write_Command(unsigned char commandValue);
void LCD_Write_Data(unsigned char dataValue);
void LCD_Wait();
void LCD_Enable();
void LCD_Disable();
void LCD_Write_String(char text[]);
void LCD_Display_ON_OFF(bool displayON, bool cursorON, bool cursorPositionON); 
void LCD_Clear();
void LCD_Home();
void LCD_ShiftDisplay(bool shiftDisplayON , bool directionRight );
// *** END of 'Private' Functions accessed by other member functions - do not call these direct from application code ***

#if 0
// *** USER functions
void LCD_WriteChar(unsigned char cValue);
void LCD_SetCursorPosition(unsigned char iColumnPosition /*0 - 40 */, unsigned char iRowPosition /*0 for top row, 1 for bottom row*/);
void LCD_WriteString(char Text[]);

#endif 


#endif

