#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "SlotMachine.h"
#include "LcdLibrary.h"
#include "KeyPad.h"


#define LIGHTS_DD  PORT_LETTER_TO_DD(LIGHTS_PORT_LETTER)
#define LIGHTS_PORT  PORT_LETTER_TO_PORT(LIGHTS_PORT_LETTER)
#define LIGHTS_PIN  PORT_LETTER_TO_PIN(LIGHTS_PORT_LETTER)

GameData gGameData;
volatile unsigned int idleTimeOut = 0;

//char spinWheelValues[] = { DOLLAR_SYMBOL,'0','1',YEN_SYMBOL,'3','4',PI_SYMBOL,HASH_SYMBOL,'5','6',SUMMATION_SYMBOL,'2','7','9','8'};
char spinWheelValues[] = { DOLLAR_SYMBOL,YEN_SYMBOL,PI_SYMBOL,HASH_SYMBOL,SUMMATION_SYMBOL};
//char spinWheelValues[] = { DOLLAR_SYMBOL,YEN_SYMBOL,PI_SYMBOL};
#define spinWheelValuesLength sizeof(spinWheelValues)/sizeof(char)

void SM_InitGameData()
{
	gGameData.smState = SM_INIT;
	gGameData.playerData.Bet = 1;
	gGameData.playerData.Balance = START_BALANCE;
	gGameData.winValue = 0;
	spinReels = SPIN_OFF;
	gGameData.stopGame = false;
	srand((unsigned int) rand());
	gGameData.wheel1Pos =  rand() % spinWheelValuesLength;
	srand((unsigned int)gGameData.wheel1Pos);
	gGameData.wheel2Pos = rand() % spinWheelValuesLength;
	srand((unsigned int)gGameData.wheel2Pos);
	gGameData.wheel3Pos = rand() % spinWheelValuesLength;
}

void SM_Init()
{
	LIGHTS_DD = 0xFF;
	SM_SystemBusyLights();
	SM_InitGameData();
    SM_UpdateLCD();
	KP_Init();
	SM_InitialiseIdleTimer();
    KP_Enable_Spin();
    KP_Enable_Bet();
	gGameData.smState = SM_USER_WAIT;
	SM_UserWaitLights();
//    KP_Enable_Bet_Max();
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
            case SUMMATION_SYMBOL:
                return SUMMATION_REWARD * gGameData.playerData.Bet;
            case PI_SYMBOL:
                return PI_REWARD * gGameData.playerData.Bet;	
            default:
                return 0;
        }
    }
	if ( (spinWheelValues[gGameData.wheel1Pos] == PI_SYMBOL && gGameData.wheel2Pos == gGameData.wheel3Pos)  || 
			( spinWheelValues[gGameData.wheel2Pos] == PI_SYMBOL && gGameData.wheel1Pos == gGameData.wheel3Pos) || 
			( spinWheelValues[gGameData.wheel3Pos] == PI_SYMBOL && gGameData.wheel1Pos == gGameData.wheel2Pos) )
	{
		return DOUBLE_MATCH_REWARD * gGameData.playerData.Bet;
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
   sprintf(lcdString,"%5d",gGameData.playerData.Balance);
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
   sprintf(lcdString,"%3d",gGameData.winValue);
   LCD_SetCursorPosition( WIN_CURSOR_COL , WIN_CURSOR_ROW);
   LCD_Write_String(lcdString);
}

void CheckPinSignal()
{
	return;
	DDRE &= ~ 0b00010000;
	if (PINE & (1<<PINE4))
	{
		spinReels = SPIN_ON;
	}
	else
	{
		spinReels = SPIN_OFF;
	}
}

void SM_SpinWheel()
{
	KP_Enable_Spin();
	int count = 1;
	CheckPinSignal(); 
	if (gGameData.smState == SM_USER_WAIT)
	{
		SM_EnableIdleTimer();
		gGameData.smState = SM_IDLE_TIMER_START;
	}
	if(SPIN_OFF == spinReels) return;
	SM_DisableIdleTimer();
	gGameData.smState = SM_SPIN;
    KP_Disable_Bet();
    KP_Disable_Bet_Max();
	SM_LightsOff();
    while(SPIN_ON == spinReels)
    {
	   KP_Enable_Spin(); 	
       gGameData.wheel1Pos = ( gGameData.wheel1Pos + 1 ) %spinWheelValuesLength;
       if (count % 2 == 0) gGameData.wheel2Pos = ( gGameData.wheel2Pos + 1 ) %spinWheelValuesLength;
       if (count % 3 == 0) gGameData.wheel3Pos = ( gGameData.wheel3Pos + 1 ) %spinWheelValuesLength;
	   if (count >= 3)
	   {
		    count = 1;
	   }
	   else
	   {
	   	   count++;
	   }
        SM_UpdateLCDReels();
		SM_SpinningLights();
       _delay_ms(SPIN_DELAY);
	   CheckPinSignal();
    }
	KP_Disable_Spin();
    

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
			_delay_ms(DISPLAY_BANNER_DELAY);
			SM_InitGameData();
			LCD_Clear();
        }
    }
    else
    {
		SM_BetWonLights();
        gGameData.playerData.Balance += gGameData.winValue;
        if (gGameData.playerData.Balance >= MAX_WIN_BALANCE)
        {
            gGameData.playerData.Balance = MAX_WIN_BALANCE;
            SM_Winner();
			gGameData.smState = SM_IDLE;
			_delay_ms(DISPLAY_BANNER_DELAY);
			SM_InitGameData();
			LCD_Clear();	
        } 

    }
    SM_UpdateLCD();
	KP_Enable_Spin();
    KP_Enable_Bet();
    KP_Enable_Bet_Max();
	gGameData.smState = SM_USER_WAIT;
	SM_UserWaitLights();
}

void SM_StopWheel()
{
    spinReels = SPIN_OFF;
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
   if( SPIN_ON == spinReels )
   {
		spinReels = SPIN_OFF;
   }
   else
   {
	   spinReels = SPIN_ON;
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

void SM_Idle()
{

	LCD_Clear();
    LCD_SetCursorPosition( IDLE_TEXT_COL , IDLE_TEXT_ROW);
    LCD_Write_String(IDLE_TEXT);
	gGameData.smState = SM_IDLE;
	SM_IdleLights();
	
}

void SM_Winner()
{
	LCD_Clear();
	LCD_SetCursorPosition( YOU_WON_TEXT_COL , YOU_WON_TEXT_ROW);
	LCD_Write_String(YOU_WON_TEXT);
	SM_WinnerLights();
}

void SM_GameOver()
{
	LCD_Clear();
	LCD_SetCursorPosition( GAMEOVER_TEXT_COL , GAMEOVER_TEXT_ROW);
	LCD_Write_String(GAMEOVER_TEXT);
	SM_GameOverLights();
}

void SM_EnableIdleTimer()
{
	// check last 3 bits , if all is zero then enable
	if((TCCR1B & 0x07) == 0x00)
	{
		//reset timer value as it may be some intermediate value when stopped
		idleTimeOut = 0;
		OPER_16_BIT_START
		TCNT1H = 0b00000000;    // Timer/Counter count/value registers (16 bit) TCNT1H and TCNT1L
		TCNT1L = 0b00000000;
		OPER_16_BIT_END
		TCCR1B = 0b00001101; // pre-scalar 1024
	}
}

void SM_DisableIdleTimer()
{
	TCCR1B = 0b00001000;
}

void SM_InitialiseIdleTimer()         // Configure to generate an interrupt after a 1-Second interval
{
	TCCR1A = 0b00000000;    // Normal port operation (OC1A, OC1B, OC1C), Clear Timer on 'Compare Match' (CTC) waveform mode)
	TCCR1B = 0b00001000;    // CTC waveform mode, initially stopped (no clock)
	TCCR1C = 0b00000000;

	// For 1 MHz clock (with 1024 prescaler) to achieve a 1 second interval:
	// Need to count 1 million clock cycles (but already divided by 1024)
	// So actually need to count to (1000000 / 1024 =) 976 decimal, = 3D0 Hex
	OPER_16_BIT_START
	OCR1AH = 0x03; // Output Compare Registers (16 bit) OCR1BH and OCR1BL
	OCR1AL = 0xD0;
	OPER_16_BIT_END

	
	TIMSK1 = 0b00000010;    // bit 1 OCIE1A         Use 'Output Compare A Match' Interrupt, i.e. generate an interrupt
	// when the timer reaches the set value (in the OCR1A register)
}

void SM_BetPressedLights()
{
	LIGHTS_PORT = 0b00000011;
}

void SM_SpinPressedLights()
{
	LIGHTS_PORT = 0b11000000;
}

void SM_SpinningLights()
{
	if (LIGHTS_PIN == 0x00)
	{
		LIGHTS_PORT = 0b10000000;
	}
	else
	{
		LIGHTS_PORT >>=1;
	}
}

void SM_UserWaitLights()
{
	LIGHTS_PORT = 0b00011000;
}

void SM_WinnerLights()
{
	LIGHTS_PORT = 0b10101010;	
}
void SM_GameOverLights()
{
	LIGHTS_PORT = 0b10000001;
}

void SM_BetWonLights()
{
	LIGHTS_PORT = 0b01100110;
}

void SM_SystemBusyLights()
{
	LIGHTS_PORT = 0xFF;
}

void SM_IdleLights()
{
	LIGHTS_PORT = 0x00;
}

void SM_LightsOff()
{
	LIGHTS_PORT = 0x00;
}


/*
ISR(INT2_vect) // Interrupt Handler for H/W INT 0
{
	KP_Disable_Bet_Max();	
    SM_BetMax();
	_delay_ms(80);		// Short delay to debounce the push-button
    KP_Enable_Bet_Max();
}

*/