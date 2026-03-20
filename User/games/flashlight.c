#include <string.h>
#include <stdlib.h>

#include "debug.h"
#include "ch32v00x.h"
#include "termGFX.h"
#include "st7735.h"
#include "buttons.h"
#include "engine.h"
#include "menu.h"


#include "flashlight.h"

static uint8_t exit_armed = 0;

void FLASHLIGHT_Init(void)
{
    tGFX_Clear(UI_THEME_FLASHLIGHT_BG);
    // Ignore the ACTION press used to enter this mode until released once.
    exit_armed = 0;

    ENG_UpdateInFrameCallback(&FLASHLIGHT_Main);
}

void FLASHLIGHT_Main(void)
{
    if(!BTN_IsPressed(BTN_ACTION))
    {
        exit_armed = 1;
    }
    else if(exit_armed)
	{
		ENG_SetInFrameCallback(&MENU_MainMenuProcess); 
		MENU_ForceBTNNotReleased(); 
		tGFX_Clear(UI_THEME_MENU_BG);
	}
}
