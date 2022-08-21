/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Ending Screen Functions Definitions (Init, Update, Draw, Unload)
*
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

//----------------------------------------------------------------------------------
// Ending Screen Functions Definition
//----------------------------------------------------------------------------------

// Ending Screen Initialization logic
void InitEndingScreen(void)
{
    // TODO: Initialize ENDING screen variables here!
    framesCounter = 0;
    finishScreen = 0;
}

// Ending Screen Update logic
void UpdateEndingScreen(void)
{
    // TODO: Update ENDING screen variables here!

    // Press enter or tap to return to TITLE screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
}

// Ending Screen Draw logic
void DrawEndingScreen(void)
{
    // TODO: Draw ENDING screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 109, 104, 117, 255});
    DrawTextEx(font, "GAME OVER ENDING_SCREEN", Vector2{ 20, 10 }, font.baseSize*3.0f, 4, Color{ 181, 131, 141, 255});
    DrawText("__todo__\nPRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, Color{ 181, 131, 141, 255 });
}

// Ending Screen Unload logic
void UnloadEndingScreen(void)
{
    // TODO: Unload ENDING screen variables here!
}

// Ending Screen should finish?
int FinishEndingScreen(void)
{
    return finishScreen;
}