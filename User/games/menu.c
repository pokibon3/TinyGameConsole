/*
 * menu.c
 *
 *  Created on: Jul 11, 2023
 *      Author: max
 */

#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "ch32v00x.h"
#include "termGFX.h"
#include "st7735.h"
#include "buttons.h"
#include "tick.h"
#include "sound.h"
#include "engine.h"
#include "pwr_monitor.h"
#include "menu.h"

#include "snake.h"
#include "pong.h"
#include "jumpbird.h"
#include "dinorun.h"
#include "highscore.h"
#include "flashlight.h"

static const menu_option_t menu[MENU_ITEMS_COUNT] =
{
		{"SNAKE", &MENU_SetGameSnake},
		{"PONG", &MENU_SetGamePong},
		{"JUMP BIRD", &MENU_SetGameJumpBird},
		{"DINO RUN", &MENU_SetGameDinoRun},
		{"HIGH SCORE", &MENU_SetGameHighscore},
		{"SOUND: ", &MENU_ChangeSound},
		{"FLASHLIGHT", &MENU_SetGameFlashlight},
};

static const uint8_t MENU_THEME_ACCENTS[MENU_ITEMS_COUNT] = {
	UI_COLOR_GREEN, UI_COLOR_BLUE, UI_COLOR_CYAN, UI_COLOR_YELLOW, UI_COLOR_MAGENTA, UI_COLOR_RED, UI_COLOR_WHITE
};
static const char *MENU_TITLE_TEXT = "MAIN MENU";

static uint8_t btn_relesed = 1;
static uint8_t selected_option;

void MENU_SetGameSnake(void)
{
	    ENG_UpdateInFrameCallback(&SNAKE_Init);
}

void MENU_SetGamePong(void)
{
	    ENG_UpdateInFrameCallback(&PONG_Init);
}

void MENU_SetGameJumpBird(void)
{
        ENG_UpdateInFrameCallback(&JUMPBIRD_Init);
}

void MENU_SetGameDinoRun(void)
{
        ENG_UpdateInFrameCallback(&DINORUN_Init);
}

void MENU_SetGameHighscore(void)
{
        ENG_UpdateInFrameCallback(&HIGHSCORE_Init);
}

void MENU_SetGameFlashlight(void)
{
		ENG_UpdateInFrameCallback(&FLASHLIGHT_Init);
}


void MENU_ChangeSound(void)
{
    if(SND_GetMuteState() == 0)
	{
		SND_Mute();
	}
	else
	{
		SND_Unmute();
		SND_PlayNow(300, 10);
	}
}

void MENU_MainMenuProcess(void)
{
	uint8_t title_x = (TERM_SIZE_X - strlen(MENU_TITLE_TEXT)) / 2;

	MENU_SelectionProcess();
	tGFX_Clear(UI_THEME_MENU_BG);

	tGFX_SetCursor(title_x, 0);
	tGFX_Print((char*)MENU_TITLE_TEXT, UI_THEME_MENU_TITLE, UI_THEME_MENU_BG);

	for(size_t i = 0; i < MENU_ITEMS_COUNT; i++)
	{

		tGFX_SetCursor(1, i + 2);

	    if(selected_option == i)
	    {
	        tGFX_Print((char*)menu[i].name, UI_THEME_MENU_SELECTED_TEXT, MENU_THEME_ACCENTS[i]);
	    }
	    else
	    {
	    	tGFX_Print((char*)menu[i].name, UI_THEME_MENU_TEXT, UI_THEME_MENU_BG);
	    }
	}

	if(SND_GetMuteState() == 0)
	{
		tGFX_SetCursor(1 + strlen(menu[MENU_SOUND_POS].name), MENU_SOUND_POS + 2);
		tGFX_Print("   ", UI_THEME_MENU_BG, UI_THEME_MENU_BG); // Clear
		tGFX_SetCursor(1 + strlen(menu[MENU_SOUND_POS].name), MENU_SOUND_POS + 2);

		if(selected_option == MENU_SOUND_POS)
		{
			tGFX_Print("ON", UI_THEME_MENU_SELECTED_TEXT, MENU_THEME_ACCENTS[MENU_SOUND_POS]);
		}
		else
		{
			tGFX_Print("ON", UI_THEME_MENU_SOUND_ON, UI_THEME_MENU_BG);
		}
	}
	else
	{
		tGFX_SetCursor(1 + strlen(menu[MENU_SOUND_POS].name), MENU_SOUND_POS + 2);

		if(selected_option == MENU_SOUND_POS)
		{
			tGFX_Print("OFF", UI_THEME_MENU_SELECTED_TEXT, MENU_THEME_ACCENTS[MENU_SOUND_POS]);
		}
		else
		{
			tGFX_Print("OFF", UI_THEME_MENU_SOUND_OFF, UI_THEME_MENU_BG);
		}		
	}

	if(PWR_Monitor_IsPowerLow() == 1)
	{
		tGFX_SetCursor(1, 9);
		tGFX_Print("!! LOW BATTERY !!", UI_THEME_MENU_LOW_BAT, UI_THEME_MENU_BG);
	}
}

void MENU_ForceBTNNotReleased(void)
{
	btn_relesed = 0;
}

void MENU_SelectionProcess(void)
{
	if(btn_relesed)
	{
		if(BTN_IsPressed(BTN_RIGHT))
		{
			btn_relesed = 0;
			if(selected_option < MENU_ITEMS_COUNT - 1) selected_option++;
			SND_PlayNow(300, 10);
		}

		if(BTN_IsPressed(BTN_LEFT))
		{
			btn_relesed = 0;
			if(selected_option != 0) selected_option--;
			SND_PlayNow(300, 10);
		}

		if(BTN_IsPressed(BTN_ACTION))
		{
			btn_relesed = 0;
			menu[selected_option].Action();

		}
	}
	else if(!BTN_IsPressed(BTN_RIGHT) && !BTN_IsPressed(BTN_LEFT) && !BTN_IsPressed(BTN_ACTION))
	{
		btn_relesed = 1;
	}
}
