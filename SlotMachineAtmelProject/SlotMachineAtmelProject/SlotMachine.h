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
#define PLAYER_BALANCE_CURSOR_COL 10

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
#define PLAYER_BALANCE_TEXT_COL 6

#define PLAYER_BET_TEXT "Bet:"
#define PLAYER_BET_TEXT_ROW LCD_ROW_1
#define PLAYER_BET_TEXT_COL 0


#define IDLE_TEXT "Press To Start"
#define IDLE_TEXT_ROW LCD_ROW_1
#define IDLE_TEXT_COL 0

#define YOU_WON_TEXT "YOU WON"
#define YOU_WON_TEXT_ROW LCD_ROW_1
#define YOU_WON_TEXT_COL 5

#define GAMEOVER_TEXT "GAMEOVER"
#define GAMEOVER_TEXT_ROW LCD_ROW_1
#define GAMEOVER_TEXT_COL 4

#define SPIN_DELAY	100
#define DISPLAY_BANNER_DELAY 10000

#define IDLE_TIME_OUT_VALUE 10

#define DOLLAR_REWARD  10 
#define YEN_REWARD  20 
#define HASH_REWARD  30 
#define SUMMATION_REWARD  50 
#define PI_REWARD  100 

#define DOUBLE_MATCH_REWARD 5


typedef enum _gameState {SM_INIT, SM_USER_WAIT, SM_IDLE_TIMER_START, SM_SPIN, SM_IDLE} GameState;

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
	volatile GameState smState;

} GameData;

volatile unsigned char spinReels;
extern volatile unsigned int idleTimeOut;
extern GameData gGameData;


void SM_InitGameData();

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
void SM_Idle();
void SM_InitialiseIdleTimer();
void SM_EnableIdleTimer();
void SM_DisableIdleTimer();

void SM_BetPressedLights();
void SM_SpinPressedLights();
void SM_SpinningLights();
void SM_UserWaitLights();
void SM_WinnerLights();
void SM_GameOverLights();
void SM_BetWonLights();
void SM_SystemBusyLights();
void SM_IdleLights();
void SM_LightsOff();


#endif
