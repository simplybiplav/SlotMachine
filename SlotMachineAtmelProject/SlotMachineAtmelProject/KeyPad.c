#include "KeyPad.h"

unsigned char buttonPressed = CLEAR_BITS ;
void KP_Init()
{
	// EICRA  External Interrupt Control Register A
	// Bits 7:0  ISC31, ISC30  ISC00, ISC00: External Interrupt 3 - 0 Sense Control Bits
	// 10  = The falling edge of INTn generates asynchronously an interrupt request
	EICRA = 0b00000010;		// Interrupt Sense (INT0) falling-edge triggered
	
	// EICRB  External Interrupt Control Register B
	// Bits 7:0  ISC71, ISC70 - ISC41, ISC40: External Interrupt 7 - 4 Sense Control Bits
	// 10  = The falling edge of INTn generates asynchronously an interrupt request
	EICRB = 0b00000000;
	
	// EIMSK  External Interrupt Mask Register
	// Bits 7:0  INT7:0: External Interrupt Request 7 - 0 Enable
	EIMSK = 0b00000000;		// Initially disabled, set bit 0 to Enable H/W Int 0
	
	// EIFR  External Interrupt Flag Register
	// Bits 7:0  INTF7:0: External Interrupt Flags 7 - 0
	EIFR = 0b11111111;		// Clear all HW interrupt flags (in case a spurious interrupt has occurred during chip startup)

    KEYPAD_DD = INPUT_MODE;
    sei();

}

void KP_Enable_Spin()
{
	EIMSK |= (1<< KEY_SPIN);
}
void KP_Enable_Bet()
{

	EIMSK |= (1<< KEY_BET);
}

void KP_Enable_Bet_Max()
{
	EIMSK |= (1<< KEY_BET_MAX);

}

void KP_Disable_Spin()
{

	EIMSK &= ~(1<< KEY_SPIN);
}

void KP_Disable_Bet()
{
	EIMSK &= ~(1<< KEY_BET);

}

void KP_Disable_Bet_Max()
{
	EIMSK &= ~(1<< KEY_BET_MAX);

}





