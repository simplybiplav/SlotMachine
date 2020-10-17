/*
 * Config.h
 *
 * Created: 21/09/2020 17:11:31
 *  Author: 230712
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>
#define LCD_IO_CMD_PORT_LETTER G
#define LCD_IO_CMD_PORT_RS 0
#define LCD_IO_CMD_PORT_RW 1
#define LCD_IO_CMD_PORT_EN 2
#define LCD_IO_DATA_PORT_LETTER A
#define KEYPAD_PORT_LETTER D
#define LIGHTS_PORT_LETTER B

#define KEY_DEBOUNCE_WAIT 100

#define INPUT_MODE 0x00
#define OUTPUT_MODE 0xFF

#define ALL_BITS 0xFF
#define CLEAR_BITS 0x00


#define PORT_LETTER_TO_DD(PORT_LETTER)  _PORT_LETTER_TO_DD(PORT_LETTER)
#define _PORT_LETTER_TO_DD(PORT_LETTER)  DDR ## PORT_LETTER

#define PORT_LETTER_TO_PORT(PORT_LETTER)  _PORT_LETTER_TO_PORT(PORT_LETTER)
#define _PORT_LETTER_TO_PORT(PORT_LETTER)  PORT ## PORT_LETTER

#define PORT_LETTER_TO_PIN(PORT_LETTER)  _PORT_LETTER_TO_PIN(PORT_LETTER)
#define _PORT_LETTER_TO_PIN(PORT_LETTER)  PIN ## PORT_LETTER


	/*
	VERY IMPORTANT
	To do a 16-bit write, the high byte must be written before the low byte. 
	For a 16-bit read, the low byte must be read before the high byte.
	
	It is important to notice that accessing 16-bit registers are atomic operations.
	If an interrupt occurs between the two instructions accessing the 16-bit register,
	and the interrupt code updates the temporary register by accessing the same or any
	other of the 16-bit Timer Registers, then the result of the access outside the 
	interrupt will be corrupted. Therefore, when both the main code and the interrupt 
	code update the temporary register, the main code must disable the interrupts during 
	the 16-bit access.
	*/


#define OPER_16_BIT_START  { unsigned char _oper_16_bit_sreg = SREG; cli();

#define OPER_16_BIT_END    SREG = _oper_16_bit_sreg; }

#endif /* CONFIG_H_ */
