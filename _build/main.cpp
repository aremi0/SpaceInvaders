/*******************************************************************************************
*
*
*   <Game title>                Space Invaders!
*   <Game description>          C++ versione of Space Invaders!
*   <Author>                    aremi
*   <date>                      2022-08-19
* 
*
********************************************************************************************/

#include "raylib.h"
#include "../_build/screen/screens.h"    // NOTE: Declares global (extern) variables and screens functions

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------

// Game resources
GameScreen currentScreen = GameScreen::LOGO;
Font font = { 0 };
Music music = { 0 };
Sound fxCoin = { 0 };

std::vector<Texture2D> allTexture;

// Player-specified resources
Player* player = nullptr;

// Bullets-specified resources
Sound fxBulletShot = { 0 };
std::vector<Bullet> enemyBullets;
std::vector<Bullet> playerBullets;

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 900;
static const int screenHeight = 680;

// Required variables to manage screen transitions (fade-in, fade-out)
static float transAlpha = 0.0f;
static bool onTransition = false;
static bool transFadeOut = false;
static GameScreen transFromScreen = GameScreen::_DEB;
static GameScreen transToScreen = GameScreen::_DEB;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void ChangeToScreen(int screen);     // Change to screen, no transition effect

static void TransitionToScreen(int screen); // Request transition to next screen
static void UpdateTransition(void);         // Update transition effect
static void DrawTransition(void);           // Draw transition effect (full-screen rectangle)

static void UpdateDrawFrame(void);          // Update and draw one frame

int loadAllTextures(void);

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Space Invaders!");
    Image icon = LoadImage("resources/img/icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);

    InitAudioDevice();                                                                  // Initialize audio device

    // Load global data (assets that must be available in all screens, i.e. font)
    font = LoadFont("resources/mecha.png");
    music = LoadMusicStream("resources/Boss_Time.mp3");
    fxCoin = LoadSound("resources/coin.wav");
    fxBulletShot = LoadSound("resources/sound/shoot.wav");
    loadAllTextures();

    SetMusicVolume(music, 0.7f);
    PlayMusicStream(music);

    // Setup and init first screen
    currentScreen = LOGO;
    InitLogoScreen();

    // Creazione degli oggetti di gioco
    player = new Player(allTexture.at(TextureIndexes::PLAYER_T));

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Unload current screen data before closing
    switch (currentScreen)
    {
        case LOGO: UnloadLogoScreen(); break;
        case TITLE: UnloadTitleScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        case ENDING: UnloadEndingScreen(); break;
        default: break;
    }

    // Unload global data loaded
    UnloadFont(font);
    UnloadMusicStream(music);
    UnloadSound(fxCoin);
    UnloadSound(fxBulletShot);

    // Unaload all textures from V-RAM
    for (auto& obj : allTexture)
        UnloadTexture(obj);
    allTexture.clear();

    CloseAudioDevice();     // Close audio context

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Load all game-texture on V-RAM
//----------------------------------------------------------------------------------
int loadAllTextures(void) {
    Image image;
    Texture2D texture;

    // Loading BACKGROUND Texture on RAM
    image = LoadImage("resources/img/background.png");
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // BACKGROUND_T => index 0
    UnloadImage(image);                                                                 // Unload image from RAM

    // Load PLAYER Texture on RAM
    image = LoadImage("resources/img/player/player.png");
    ImageColorTint(&image, GREEN);
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // PLAYER_T => index 1
    UnloadImage(image);                                                                 // Unload image from RAM

    // Load PLAYER_BULLET Texture on RAM
    image = LoadImage("resources/img/player/bullet/player_bullet.png");
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // PLAYER_BULLET_T => index 2
    UnloadImage(image);                                                                 // Unload image from RAM

    // Load PLAYER_BULLET_EXPLODING Texture on RAM
    image = LoadImage("resources/img/player/bullet/player_bullet_exploding.png");
    ImageColorTint(&image, RED);
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // PLAYER_BULLET_EXPLODING_T => index 3
    UnloadImage(image);                                                                 // Unload image from RAM

    return 1;
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
// Change to next screen, no transition
static void ChangeToScreen(GameScreen screen)
{
    // Unload current screen
    switch (currentScreen)
    {
        case LOGO: UnloadLogoScreen(); break;
        case TITLE: UnloadTitleScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        case ENDING: UnloadEndingScreen(); break;
        default: break;
    }

    // Init next screen
    switch (screen)
    {
        case LOGO: InitLogoScreen(); break;
        case TITLE: InitTitleScreen(); break;
        case GAMEPLAY: InitGameplayScreen(); break;
        case ENDING: InitEndingScreen(); break;
        default: break;
    }

    currentScreen = screen;
}

// Request transition to next screen
static void TransitionToScreen(GameScreen screen)
{
    onTransition = true;
    transFadeOut = false;
    transFromScreen = currentScreen;
    transToScreen = screen;
    transAlpha = 0.0f;
}

// Update transition effect (fade-in, fade-out)
static void UpdateTransition(void)
{
    if (!transFadeOut)
    {
        transAlpha += 0.05f;

        // NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
        // For that reason we compare against 1.01f, to avoid last frame loading stop
        if (transAlpha > 1.01f)
        {
            transAlpha = 1.0f;

            // Unload current screen
            switch (transFromScreen)
            {
                case LOGO: UnloadLogoScreen(); break;
                case TITLE: UnloadTitleScreen(); break;
                case OPTIONS: UnloadOptionsScreen(); break;
                case GAMEPLAY: UnloadGameplayScreen(); break;
                case ENDING: UnloadEndingScreen(); break;
                default: break;
            }

            // Load next screen
            switch (transToScreen)
            {
                case LOGO: InitLogoScreen(); break;
                case TITLE: InitTitleScreen(); break;
                case GAMEPLAY: InitGameplayScreen(); break;
                case ENDING: InitEndingScreen(); break;
                default: break;
            }

            currentScreen = transToScreen;

            // Activate fade out effect to next loaded screen
            transFadeOut = true;
        }
    }
    else  // Transition fade out logic
    {
        transAlpha -= 0.02f;

        if (transAlpha < -0.01f)
        {
            transAlpha = 0.0f;
            transFadeOut = false;
            onTransition = false;
            transFromScreen = GameScreen::_DEB;
            transToScreen = GameScreen::_DEB;
        }
    }
}

// Draw transition effect (full-screen rectangle)
static void DrawTransition(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    UpdateMusicStream(music);       // NOTE: Music keeps playing between screens

    if (!onTransition)
    {
        switch(currentScreen)
        {
            case LOGO:
            {
                UpdateLogoScreen();

                if (FinishLogoScreen() == 1) TransitionToScreen(TITLE);
                else if (FinishLogoScreen() == 2) TransitionToScreen(GAMEPLAY);

            } break;
            case TITLE:
            {
                UpdateTitleScreen();

                if (FinishTitleScreen() == 1) TransitionToScreen(OPTIONS);
                else if (FinishTitleScreen() == 2) TransitionToScreen(GAMEPLAY);

            } break;
            case OPTIONS:
            {
                UpdateOptionsScreen();

                if (FinishOptionsScreen()) TransitionToScreen(TITLE);

            } break;
            case GAMEPLAY:
            {
                UpdateGameplayScreen(player, enemyBullets, playerBullets);

                if (FinishGameplayScreen() == 1) TransitionToScreen(ENDING);
                //else if (FinishGameplayScreen() == 2) TransitionToScreen(TITLE);

            } break;
            case ENDING:
            {
                UpdateEndingScreen();

                if (FinishEndingScreen() == 1) TransitionToScreen(TITLE);

            } break;
            default: break;
        }
    }
    else UpdateTransition();    // Update transition (fade-in, fade-out)
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(RAYWHITE);

        switch(currentScreen)
        {
            case LOGO: DrawLogoScreen(); break;
            case TITLE: DrawTitleScreen(); break;
            case OPTIONS: DrawOptionsScreen(); break;
            case GAMEPLAY: DrawGameplayScreen(player, enemyBullets, playerBullets); break;
            case ENDING: DrawEndingScreen(); break;
            default: break;
        }

        // Draw full screen rectangle in front of everything
        if (onTransition) DrawTransition();

        //DrawFPS(10, 10);

    EndDrawing();
    //----------------------------------------------------------------------------------
}
