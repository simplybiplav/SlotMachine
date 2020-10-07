#ifndef _SLOT_MACHINE_H
#define _SLOT_MACHINE_H

#include <stdint.h>
#include <stdbool.h>
#define START_BALANCE 2000
#define MAX_BET 3
#define MIN_BET 1

#define SPIN_ON 1
#define SPIN_OFF 0

#define MAX_WIN_BALANCE 10000

#define REEL_CURSOR_ROW  LCD_ROW_2
#define REEL_CURSOR_COL  12

#define WIN_CURSOR_ROW  LCD_ROW_2
#define WIN_CURSOR_COL  4

#define PLAYER_BALANCE_CURSOR_ROW LCD_ROW_1
#define PLAYER_BALANCE_CURSOR_COL 11

#define PLAYER_BET_CURSOR_ROW LCD_ROW_1
#define PLAYER_BET_CURSOR_COL 4

#define REEL_TEXT_LEFT 0x7E
#define REEL_TEXT_LEFT_ROW  LCD_ROW_2
#define REEL_TEXT_LEFT_COL  11

#define REEL_TEXT_RIGHT 0x7F
#define REEL_TEXT_RIGHT_ROW  LCD_ROW_2
#define REEL_TEXT_RIGHT_COL  15

#define WIN_TEXT "Won:"
#define WIN_TEXT_ROW  LCD_ROW_2
#define WIN_TEXT_COL  0

#define PLAYER_BALANCE_TEXT "Bal:"
#define PLAYER_BALANCE_TEXT_ROW LCD_ROW_1
#define PLAYER_BALANCE_TEXT_COL 7

#define PLAYER_BET_TEXT "Bet:"
#define PLAYER_BET_TEXT_ROW LCD_ROW_1
#define PLAYER_BET_TEXT_COL 0


#define DOLLAR_REWARD  5 
#define YEN_REWARD  20 
#define HASH_REWARD  30 
#define SEVEN_REWARD  50 
#define SUMMATION_REWARD  70 
#define PI_REWARD  100 

typedef struct _playerData {
    uint16_t Balance;
    volatile uint16_t Bet;
} PlayerData;



typedef struct _gameData {
    PlayerData playerData;
    uint16_t winValue;
    volatile bool stopGame;
    unsigned wheel1Pos;
    unsigned wheel2Pos;
    unsigned wheel3Pos;

} GameData;

volatile unsigned char spinReels;
extern GameData gGameData;

void SM_ToggleSpin();
void SM_BetMax();
void SM_IncreaseBet();
void SM_Init();
uint16_t SM_WinValue();
void SM_UpdateLCDPlayerBet();
void SM_UpdateLCDPlayerBalance();
void SM_UpdateLCDReels();
void SM_UpdateLCD();
void SM_UpdateLCDValue();
void SM_UpdateLCDTexts();
void SM_UpdateLCDWinValue();
void SM_SpinWheel();
void SM_StopWheel();
void SM_Winner();
void SM_GameOver();
#endif
