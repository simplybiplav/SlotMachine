#ifndef _KEY_PAD_H
#define _KEY_PAD_H

#include "Config.h"
#include <avr/interrupt.h>

#define KEYPAD_DD PORT_LETTER_TO_DD(KEYPAD_PORT_LETTER)
#define KEYPAD_PORT PORT_LETTER_TO_PORT(KEYPAD_PORT_LETTER)
#define KEYPAD_PIN PORT_LETTER_TO_PIN(KEYPAD_PORT_LETTER)

#define KEY_SPIN 0
#define KEY_BET 1
#define KEY_BET_MAX 2


extern unsigned char buttonPressed;
void KP_Init();
void KP_Enable_Spin();
void KP_Enable_Bet();
void KP_Enable_Bet_Max();
void KP_Disable_Spin();
void KP_Disable_Bet();
void KP_Disable_Bet_Max();
#endif
