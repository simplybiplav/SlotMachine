
#ifndef _SLOT_MACHINE_H
#define _SLOT_MACHINE_H

#define START_BALANCE 2000
#define MAX_BET 3

#define MAX_WIN_BALANCE 10000

#define REEL_CURSOR_ROW  LCD_ROW_2
#define REEL_CURSOR_COL  12

#define WIN_CURSOR_ROW  LCD_ROW_2
#define WIN_CURSOR_COL  12

#define PLAYER_BALANCE_CURSOR_ROW LCD_ROW_1
#define PLAYER_BALANCE_CURSOR_COL 10

#define PLAYER_BET_CURSOR_ROW LCD_ROW_1
#define PLAYER_BET_CURSOR_COL 0



#define DOLLAR_REWARD  5 
#define YEN_REWARD  20 
#define HASH_REWARD  30 
#define SEVEN_REWARD  50 
#define SUMMATION_REWARD  70 
#define PI_REWARD  100 

typedef struct _playerData {
    uint16_t Balance;
    uint16_t Bet;
} PlayerData;



typedef struct _gameData {
    PlayerData playerData;
    uint16_t winValue;
    bool spinReels;
    bool stopGame;
    unsigned wheel1Pos;
    unsigned wheel2Pos;
    unsigned wheel3Pos;

} GameData;

extern GameData gGameData;


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
#endif
