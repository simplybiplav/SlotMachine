#include "LcdLibrary.h"



bool LCD_Init(bool twoLineMode,
                bool largeFontMode)
{
	MY_TEST_1(MYPORT) = 0xFF;
	_LCD_IO_2_PIN(LCD_IO_2ND_PORT_LETTER) = x00;
	LCD_IO_1_DD = 0xFF;  // configure i/o port 1 for output
    LCD_IO_1_PORT = 0x00; // clear i/o port 1
    LCD_IO_2_DD = 0xFF; // configure i/o port 2 for output
    LCD_IO_2_PORT = 0x00; // clear i/o port 1
}
