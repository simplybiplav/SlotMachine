#include "SlotMachine.h"
#include "LcdLibrary.h"

GameData gGameData;

char spinWheelValues[] = { DOLLAR_SYMBOL,0,1,YEN_SYMBOL,3,4,PI_SYMBOL,HASH_SYMBOL,5,6,SUMMATION_SYMBOL,7,1,0,9,8,3,8,4,5,6,9};

#define spinWheelValuesLength sizeof(spinWheelValues)/sizeof(char)

void SM_Init()
{
    gGameData.playerData.Bet = 1;
    gGameData.playerData.Balance = START_BALANCE;
    gGameData.winValue = 0;
    gGameData.spinReels = false;
    gGameData.stopGame = false;
    gGameData.wheel1Pos = srand(rand()) % spinWheelValuesLength;
    gGameData.wheel2Pos = srand(gGameData.wheel1Pos) % spinWheelValuesLength;
    gGameData.wheel3Pos = srand(gGameData.wheel3Pos) % spinWheelValuesLength;
}


uint16_t SM_WinValue()
{
    if( (gGameData.wheel1Pos == gGameData.wheel2Pos) && (gGameData.wheel1Pos == gGameData.wheel3Pos) )
    {
        switch (spinWheelValues[gGameData.wheel1Pos])
        {
            case DOLLAR_SYMBOL:
                return DOLLAR_REWARD * gGameData.playerData.Bet;
            case YEN_SYMBOL:
                return YEN_REWARD * gGameData.playerData.Bet;
            case HASH_SYMBOL:
                return HASH_REWARD * gGameData.playerData.Bet;
            case 7:
                return SEVEN_REWARD * gGameData.playerData.Bet;
            case SUMMATION_SYMBOL:
                return SUMMATION_REWARD * gGameData.playerData.Bet;
            case PI_SYMBOL:
                return PI_REWARD * gGameData.playerData.Bet;
            case default:
                return 0;
        }
    }
    return 0;

}

void SM_UpdateLCDPlayerBet()
{
   char lcdString[10]= {'\0'};
   sprintf(lcdString,"%d",gGameData.playerData.Bet);
   LCD_SetCursorPosition( PLAYER_BET_CURSOR_COL , PLAYER_BET_CURSOR_ROW);
   LCD_WriteString(lcdString);
}

void SM_UpdateLCDPlayerBalance()
{

   char lcdString[10]= {'\0'};
   sprintf(lcdString,"%d",gGameData.playerData.Balance);
   LCD_SetCursorPosition( PLAYER_BALANCE_CURSOR_COL , PLAYER_BALANCE_CURSOR_ROW);
   LCD_WriteString(lcdString);
}

void SM_UpdateLCDReels()
{
   char reelValues[4]= {'\0'};
   reelValues[0] = spinWheelValues[gGameData.wheel1Pos]; 
   reelValues[1] = spinWheelValues[gGameData.wheel2Pos]; 
   reelValues[2] = spinWheelValues[gGameData.wheel3Pos]; 
   LCD_SetCursorPosition( REEL_CURSOR_COL , REEL_CURSOR_ROW);
   LCD_WriteString(reelValues);
}

void SM_UpdateLCDWinValue()
{

   char lcdString[10]= {'\0'};
   sprintf(lcdString,"%d",gGameData.winValue);
   LCD_SetCursorPosition( WIN_CURSOR_COL , WIN_CURSOR_ROW);
   LCD_WriteString(lcdString);
}

void SM_UpdateLCD()
{
    SM_UpdateLCDPlayerBet();
    SM_UpdateLCDPlayerBalance();
    SM_UpdateLCDReels();
    SM_UpdateLCDWinValue();
} 

void SM_SpinWheel()
{
    gGameData.spinReels = true; 
    while(true == gGameData.spinReels)
    {
       gGameData.wheel1Pos = ( gGameData.wheel1Pos + 1 ) %spinWheelValuesLength;
       gGameData.wheel2Pos = ( gGameData.wheel2Pos + 1 ) %spinWheelValuesLength;
       gGameData.wheel3Pos = ( gGameData.wheel3Pos + 1 ) %spinWheelValuesLength;
        SM_UpdateLCDReels();
       _delay_ms(10);
    }
    
    gGameData.winValue = SM_WinValue();
    if (gGameData.winValue == 0 )
    {
        if (gGameData.playerData.Balance >= gGameData.playerData.Bet)
        {
            gGameData.playerData.Balance -= gGameData.playerData.Bet;
        }
        else
        {
            gGameData.playerData.Balance = 0;
        }
    }
    else
    {
        gGameData.playerData.Balance += gGameData.winValue;
        if (gGameData.playerData.Balance >= MAX_WIN_BALANCE)
        {
            gGameData.playerData.Balance = MAX_WIN_BALANCE;
        } 

    }
    gGameData.winValue = SM_WinValue();
    SM_UpdateLCD();
}

void SM_StopWheel()
{
    gGameData.spinReels = false;
}
void SM_UpdateLCDValue()
{

}

void SM_UpdateLCDTexts()
{

}