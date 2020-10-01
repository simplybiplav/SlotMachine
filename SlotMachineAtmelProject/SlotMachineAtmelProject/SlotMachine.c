#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "SlotMachine.h"
#include "LcdLibrary.h"
#include "KeyPad.h"




GameData gGameData;

char spinWheelValues[] = { DOLLAR_SYMBOL,'0','1',YEN_SYMBOL,'3','4',PI_SYMBOL,HASH_SYMBOL,'5','6',SUMMATION_SYMBOL,'7','1','0','9','8','3','8','4','5','6','9'};

#define spinWheelValuesLength sizeof(spinWheelValues)/sizeof(char)

void SM_Init()
{
    gGameData.playerData.Bet = 1;
    gGameData.playerData.Balance = START_BALANCE;
    gGameData.winValue = 0;
    gGameData.spinReels = false;
    gGameData.stopGame = false;
	 srand((unsigned int) rand());
    gGameData.wheel1Pos =  rand() % spinWheelValuesLength;
	srand((unsigned int)gGameData.wheel1Pos);
    gGameData.wheel2Pos = rand() % spinWheelValuesLength;
	srand((unsigned int)gGameData.wheel2Pos);
    gGameData.wheel3Pos = rand() % spinWheelValuesLength;
    SM_UpdateLCD();
    KP_Enable_Spin();
    KP_Enable_Bet();
    KP_Enable_Bet_Max();
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
            default:
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
   LCD_Write_String(lcdString);
}

void SM_UpdateLCDPlayerBalance()
{

   char lcdString[10]= {'\0'};
   sprintf(lcdString,"%d",gGameData.playerData.Balance);
   LCD_SetCursorPosition( PLAYER_BALANCE_CURSOR_COL , PLAYER_BALANCE_CURSOR_ROW);
   LCD_Write_String(lcdString);
}

void SM_UpdateLCDReels()
{
   char reelValues[4]= {'\0'};
   reelValues[0] = spinWheelValues[gGameData.wheel1Pos]; 
   reelValues[1] = spinWheelValues[gGameData.wheel2Pos]; 
   reelValues[2] = spinWheelValues[gGameData.wheel3Pos]; 
   LCD_SetCursorPosition( REEL_CURSOR_COL , REEL_CURSOR_ROW);
   LCD_Write_String(reelValues);
}

void SM_UpdateLCDWinValue()
{

   char lcdString[10]= {'\0'};
   sprintf(lcdString,"%d",gGameData.winValue);
   LCD_SetCursorPosition( WIN_CURSOR_COL , WIN_CURSOR_ROW);
   LCD_Write_String(lcdString);
}

void CheckPinSignal()
{
	DDRE &= ~ 0b00010000;
	if (PINE & (1<<PINE4))
	{
		gGameData.spinReels = true;
	}
	else
	{
		gGameData.spinReels = false;
	}
}

void SM_SpinWheel()
{
    //gGameData.spinReels = true;
	CheckPinSignal(); 
	if(false == gGameData.spinReels) return;
    KP_Disable_Bet();
    KP_Disable_Bet_Max();
    while(true == gGameData.spinReels)
    {
       gGameData.wheel1Pos = ( gGameData.wheel1Pos + 1 ) %spinWheelValuesLength;
       gGameData.wheel2Pos = ( gGameData.wheel2Pos + 1 ) %spinWheelValuesLength;
       gGameData.wheel3Pos = ( gGameData.wheel3Pos + 1 ) %spinWheelValuesLength;
        SM_UpdateLCDReels();
       _delay_ms(10);
	   CheckPinSignal();
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
        if ( 0 == gGameData.playerData.Balance)
        {
            
            SM_GameOver();
        }
    }
    else
    {
        gGameData.playerData.Balance += gGameData.winValue;
        if (gGameData.playerData.Balance >= MAX_WIN_BALANCE)
        {
            gGameData.playerData.Balance = MAX_WIN_BALANCE;
            SM_Winner();
        } 

    }
    gGameData.winValue = SM_WinValue();
    SM_UpdateLCD();
    KP_Enable_Bet();
    KP_Enable_Bet_Max();
}

void SM_StopWheel()
{
    gGameData.spinReels = false;
}


void SM_UpdateLCD()
{
    SM_UpdateLCDTexts();
    SM_UpdateLCDValue();
}
 

void SM_UpdateLCDValue()
{
    SM_UpdateLCDPlayerBet();
    SM_UpdateLCDPlayerBalance();
    SM_UpdateLCDReels();
    SM_UpdateLCDWinValue();
}

void SM_UpdateLCDTexts()
{
	LCD_SetCursorPosition( WIN_TEXT_COL , WIN_TEXT_ROW);
	LCD_Write_String(WIN_TEXT);
   
	LCD_SetCursorPosition( PLAYER_BET_TEXT_COL ,PLAYER_BET_TEXT_ROW);
	LCD_Write_String(PLAYER_BET_TEXT);
	
	LCD_SetCursorPosition( PLAYER_BALANCE_TEXT_COL ,PLAYER_BALANCE_TEXT_ROW);
	LCD_Write_String(PLAYER_BALANCE_TEXT);
	
	LCD_SetCursorPosition(REEL_TEXT_LEFT_COL,REEL_TEXT_LEFT_ROW);
	LCD_Write_Data(REEL_TEXT_LEFT);
	
	LCD_SetCursorPosition(REEL_TEXT_RIGHT_COL,REEL_TEXT_RIGHT_ROW);
	LCD_Write_Data(REEL_TEXT_RIGHT);
}

void SM_ToggleSpin()
{
   if( true == gGameData.spinReels )
   {
		gGameData.spinReels = false;
   }
   else
   {
	   gGameData.spinReels = true;
   }
}
void SM_BetMax()
{
    gGameData.playerData.Bet = MAX_BET; 
}

void SM_IncreaseBet()
{
    if (gGameData.playerData.Bet == MAX_BET)
    {
        gGameData.playerData.Bet = MIN_BET;
    }
    else
    {
        gGameData.playerData.Bet += 1;
    }
}

void SM_Winner()
{

}

void SM_GameOver()
{


}

ISR(INT0_vect) // Interrupt Handler for H/W INT 0
{
	KP_Disable_Spin();	
    SM_ToggleSpin();
	_delay_ms(80);		// Short delay to debounce the push-button
    KP_Enable_Spin();
}

/*

ISR(INT1_vect) // Interrupt Handler for H/W INT 0
{
	KP_Disable_Bet();	
    SM_IncreaseBet();
	_delay_ms(80);		// Short delay to debounce the push-button
    KP_Enable_Bet();
}


ISR(INT2_vect) // Interrupt Handler for H/W INT 0
{
	KP_Disable_Bet_Max();	
    SM_BetMax();
	_delay_ms(80);		// Short delay to debounce the push-button
    KP_Enable_Bet_Max();
}

*/