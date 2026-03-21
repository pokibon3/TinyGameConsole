/*
 * gameover.c
 *
 *  Created on: Sep 19, 2024
 *      Author: Max
 */

#include <string.h>
#include <stdlib.h>

#include "debug.h"
#include "ch32v00x.h"
#include "termGFX.h"
#include "st7735.h"
#include "buttons.h"
#include "tick.h"
#include "sound.h"
#include "engine.h"
#include "menu.h"

#include  "gameover.h"

static gameover_type_t gameover_type;
static uint32_t gameover_score;
static uint32_t gameover_highscore;
static buttons_names_t gameover_button;
static char *gameover_msg;
static uint8_t frames_passed;
static uint8_t blink;

static const char* btn_strings[] = {"action", "up", "down", "right", "left"};
static const char *GAMEOVER_TITLE_TEXT = "GAME OVER";


void GAMEOVER_Main(void)
{
    // Might need modification based on max score in games.
    char score_buf[8];
    uint8_t offset = 0;
    uint8_t title_x = (TERM_SIZE_X - strlen(GAMEOVER_TITLE_TEXT)) / 2;

    frames_passed++;

    tGFX_Clear(UI_THEME_GAMEOVER_BG);

    tGFX_SetCursor(title_x, 0);
    tGFX_Print((char*)GAMEOVER_TITLE_TEXT, UI_THEME_GAMEOVER_TITLE, UI_THEME_GAMEOVER_BG);
    tGFX_SetCursor(0, 2);

    switch(gameover_type)
    {
        case GAMEOVER_TYPE_SCORE:

            itoa(gameover_score, score_buf, 10);
            tGFX_Print(">SCORE:", UI_THEME_GAMEOVER_LABEL, UI_THEME_GAMEOVER_BG);
            tGFX_Print(score_buf, UI_THEME_GAMEOVER_VALUE, UI_THEME_GAMEOVER_BG);

            tGFX_SetCursor(0, 3);
            itoa(gameover_highscore, score_buf, 10);
            tGFX_Print(">HIGH SCORE:", UI_THEME_GAMEOVER_LABEL, UI_THEME_GAMEOVER_BG);
            tGFX_Print(score_buf, UI_THEME_GAMEOVER_VALUE, UI_THEME_GAMEOVER_BG);

            offset++;

            break;

        case GAMEOVER_TYPE_MESSAGE:
            tGFX_Print(">", UI_THEME_GAMEOVER_LABEL, UI_THEME_GAMEOVER_BG);
            tGFX_Print(gameover_msg, UI_THEME_GAMEOVER_VALUE, UI_THEME_GAMEOVER_BG);
            break;

        default:
            break;

    }

    // Leave one blank line between score/message area and the action hint.
    tGFX_SetCursor(0, 4 + offset);
    tGFX_Print(">Press Action Key!", UI_THEME_GAMEOVER_HINT, UI_THEME_GAMEOVER_BG);
    tGFX_SetCursor(0, 6 + offset);
    tGFX_Print(">", UI_THEME_GAMEOVER_LABEL, UI_THEME_GAMEOVER_BG);

    if(frames_passed == 15)
    {
        frames_passed = 0;
        blink++;
    }

    if(blink%2) tGFX_SetChar('_', 1, 6 + offset, UI_THEME_GAMEOVER_CURSOR, UI_THEME_GAMEOVER_BG);
    else tGFX_SetChar(0, 1, 6 + offset, UI_THEME_GAMEOVER_BG, UI_THEME_GAMEOVER_BG);

    if(BTN_IsPressed(gameover_button)) {ENG_SetInFrameCallback(&MENU_MainMenuProcess); MENU_ForceBTNNotReleased(); tGFX_Clear(UI_THEME_MENU_BG);}
}

void GAMEOVER_Setup(gameover_type_t type, uint32_t score, uint32_t highscore, buttons_names_t button, const char *msg)
{
    gameover_type = type;
    gameover_score = score;
    gameover_highscore = highscore;
    gameover_button = button;
    gameover_msg = (char*)msg;
    frames_passed = 0;
    blink = 0;
}

void GAMEOVER_Show(void)
{
    ENG_UpdateInFrameCallback(&GAMEOVER_Main);
}


