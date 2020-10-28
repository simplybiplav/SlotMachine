#ifndef _LCD_LIBRARY_H
#define _LCD_LIBRARY_H

#include "config.h"

#if !defined(LCD_IO_1ST_PORT_LETTER) || !defined(LCD_IO_2ND_PORT_LETTER)
#error "LCD_IO_1ST_PORT_LETTER and LCD_IO_2ND_PORT_LETTER not defined"
#endif


#define LCD_IO_1_DD  PORT_LETTER_TO_DD(LCD_IO_1ST_PORT_LETTER)
#define LCD_IO_1_PORT  PORT_LETTER_TO_PORT(LCD_IO_1ST_PORT_LETTER)
#define LCD_IO_1_PIN  PORT_LETTER_TO_PIN(LCD_IO_1ST_PORT_LETTER) 
#define LCD_IO_2_DD  PORT_LETTER_TO_DD(LCD_IO_2ND_PORT_LETTER)
#define LCD_IO_2_PORT PORT_LETTER_TO_PORT(LCD_IO_2ND_PORT_LETTER)
#define LCD_IO_2_PIN  PORT_LETTER_TO_PIN(LCD_IO_2ND_PORT_LETTER) 
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

 
void LCD_Write_CommandOrData(bool bCommand /*true = Command, false = Data*/, unsigned char DataOrCommand_Value);
void LCD_Wait();
void LCD_Enable();
void LCD_Disable();
// *** END of 'Private' Functions accessed by other member functions - do not call these direct from application code ***

// *** USER functions
void LCD_Initilise(bool bTwoLine/*false = 1 line mode, true =  2 line mode*/, bool bLargeFont/*false = 5*8pixels, true = 5*11 pixels*/);
void LCD_Display_ON_OFF(bool bDisplayON /*true = ON, false = OFF*/, bool bCursorON, bool bCursorPositionON); // Turn the LCD display ON / OFF
void LCD_Clear();
void LCD_Home();
void LCD_WriteChar(unsigned char cValue);
void LCD_ShiftDisplay(bool bShiftDisplayON /*true = On false = OFF*/, bool bDirectionRight /*true = shift right, false = shift left*/);
void LCD_SetCursorPosition(unsigned char iColumnPosition /*0 - 40 */, unsigned char iRowPosition /*0 for top row, 1 for bottom row*/);
void LCD_WriteString(char Text[]);




#endif
