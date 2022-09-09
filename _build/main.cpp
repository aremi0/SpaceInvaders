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
float SCREEN_WIDTH_MARGIN = 0;
float SCREEN_HEIGHT_MARGIN = 0;
GameScreen currentScreen = GameScreen::LOGO;
Font font = { 0 };
Music music = { 0 };
Sound fxCoin = { 0 };
std::vector<Texture2D> allTexture;

// Player-specified resources
Sound fxBulletShot = { 0 };
Player* player = nullptr;
std::vector<Bullet> playerBullets;

// Enemy-specified resources
Sound fxEnemyExplosion = { 0 };
std::vector<Enemy> enemies;
std::vector<Bullet> enemyBullets;

std::array<Enemy*, 55> eny; // Matrix 11x5 builded with <array>

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 540;
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
    SCREEN_WIDTH_MARGIN = GetScreenWidth() * 0.01;
    SCREEN_HEIGHT_MARGIN = GetScreenHeight() * 0.10;
    Image icon = LoadImage("resources/img/icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);

    InitAudioDevice();                      // Initialize audio device

    // Load global data (assets that must be available in all screens, i.e. font)
    font = LoadFont("resources/mecha.png");
    music = LoadMusicStream("resources/Boss_Time.mp3");
    fxCoin = LoadSound("resources/coin.wav");
    fxBulletShot = LoadSound("resources/sound/shoot.wav");
    fxEnemyExplosion = LoadSound("resources/sound/invaderkilled.wav");
    loadAllTextures();

    SetMusicVolume(music, 1.0f);
    PlayMusicStream(music);

    // Setup and init first screen
    currentScreen = LOGO;
    InitLogoScreen();

    // Game object's creation
    //----------------------------------------------------------------------------------
    player = new Player(allTexture.at(TextureIndexes::PLAYER_T));

    /***    Enemies array structure;
    * 
    * | [squid][crab][crab][octo][octo] | [squid][crab][crab][octo][octo] | [squid][...] |
    *       0     1     2    3      4         5     6     7     8     9        10   ...
    * 
    * Accessing formula: enemies[column * n_rows + row]
    */
    for (int x = 0; x < MAX_ENEMIES_COLUMN; x++) {
        eny[x * 5 + 0] = new Enemy(allTexture.at(TextureIndexes::ENEMY_SQUID_1_T), allTexture.at(TextureIndexes::ENEMY_SQUID_2_T),
            allTexture.at(TextureIndexes::ENEMY_EXPLODING_T), x + 1, 1, EnemyType::SQUID);

        eny[x * 5 + 1] = new Enemy(allTexture.at(TextureIndexes::ENEMY_CRAB_1_T), allTexture.at(TextureIndexes::ENEMY_CRAB_2_T),
            allTexture.at(TextureIndexes::ENEMY_EXPLODING_T), x + 1, 2, EnemyType::CRAB);

        eny[x * 5 + 2] = new Enemy(allTexture.at(TextureIndexes::ENEMY_CRAB_1_T), allTexture.at(TextureIndexes::ENEMY_CRAB_2_T),
            allTexture.at(TextureIndexes::ENEMY_EXPLODING_T), x + 1, 2, EnemyType::CRAB);

        eny[x * 5 + 3] = new Enemy(allTexture.at(TextureIndexes::ENEMY_OCTOPUS_1_T), allTexture.at(TextureIndexes::ENEMY_OCTOPUS_2_T),
            allTexture.at(TextureIndexes::ENEMY_EXPLODING_T), x + 1, 4, EnemyType::OCTOPUS);

        eny[x * 5 + 4] = new Enemy(allTexture.at(TextureIndexes::ENEMY_OCTOPUS_1_T), allTexture.at(TextureIndexes::ENEMY_OCTOPUS_2_T),
        allTexture.at(TextureIndexes::ENEMY_EXPLODING_T), x + 1, 5, EnemyType::OCTOPUS);
    }


    // First Row of enemies...
    for (int x = 1; x <= MAX_ENEMIES_COLUMN; x++) {
        enemies.push_back(Enemy(allTexture.at(TextureIndexes::ENEMY_SQUID_1_T), allTexture.at(TextureIndexes::ENEMY_SQUID_2_T),
            allTexture.at(TextureIndexes::ENEMY_EXPLODING_T), x, 1, EnemyType::SQUID));

        enemies.push_back(Enemy(allTexture.at(TextureIndexes::ENEMY_CRAB_1_T), allTexture.at(TextureIndexes::ENEMY_CRAB_2_T),
            allTexture.at(TextureIndexes::ENEMY_EXPLODING_T), x, 2, EnemyType::CRAB));

        enemies.push_back(Enemy(allTexture.at(TextureIndexes::ENEMY_CRAB_1_T), allTexture.at(TextureIndexes::ENEMY_CRAB_2_T),
            allTexture.at(TextureIndexes::ENEMY_EXPLODING_T), x, 3, EnemyType::CRAB));

        enemies.push_back(Enemy(allTexture.at(TextureIndexes::ENEMY_OCTOPUS_1_T), allTexture.at(TextureIndexes::ENEMY_OCTOPUS_2_T),
            allTexture.at(TextureIndexes::ENEMY_EXPLODING_T), x, 4, EnemyType::OCTOPUS));

        enemies.push_back(Enemy(allTexture.at(TextureIndexes::ENEMY_OCTOPUS_1_T), allTexture.at(TextureIndexes::ENEMY_OCTOPUS_2_T),
            allTexture.at(TextureIndexes::ENEMY_EXPLODING_T), x, 5, EnemyType::OCTOPUS));
    }
    

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);                       // Set our game to run at 60 frames-per-second

    // Main game loop
    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose())            // Detect window close button or ESC key
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
    UnloadSound(fxEnemyExplosion);

    // Unaload all textures from V-RAM
    for (auto& obj : allTexture)
        UnloadTexture(obj);
    allTexture.clear();

    CloseAudioDevice();                     // Close audio context

    CloseWindow();                          // Close window and OpenGL context

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
    UnloadImage(image);

// PLAYER TEXTURES
//----------------------------------------------------------------------------------
    // PLAYER
    image = LoadImage("resources/img/player/player.png");
    ImageColorTint(&image, GREEN);
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // PLAYER_T => index 1
    UnloadImage(image);

    // PLAYER_EXPLODING_1
    image = LoadImage("resources/img/player/player_exploding_1.png");
    ImageColorTint(&image, GREEN);
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // PLAYER_EXPLODING_1_T => index 2
    UnloadImage(image);

    // PLAYER_EXPLODING_2
    image = LoadImage("resources/img/player/player_exploding_2.png");
    ImageColorTint(&image, GREEN);
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // PLAYER_EXPLODING_2_T => index 2
    UnloadImage(image);

    // PLAYER_BULLET
    image = LoadImage("resources/img/player/bullet/player_bullet.png");
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // PLAYER_BULLET_T => index 4
    UnloadImage(image);

    // PLAYER_BULLET_EXPLODING
    image = LoadImage("resources/img/player/bullet/player_bullet_exploding.png");
    ImageColorTint(&image, RED);
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // PLAYER_BULLET_EXPLODING_T => index 5
    UnloadImage(image);

// ENEMIES TEXTURES
//----------------------------------------------------------------------------------
    // ENEMY_EXPLODING
    image = LoadImage("resources/img/enemies/enemy_exploding.png");
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // ENEMY_EXPLODING_T => index 6
    UnloadImage(image);

    // ENEMY_UFO
    image = LoadImage("resources/img/enemies/0_UFO_Boss/ufo.png");
    ImageColorTint(&image, RED);
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // ENEMY_UFO_T => index 7
    UnloadImage(image);

    // ENEMY_UFO_EXPLODING
    image = LoadImage("resources/img/enemies/0_UFO_Boss/ufo_exploding.png");
    ImageColorTint(&image, RED);
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // ENEMY_UFO_T => index 8
    UnloadImage(image);

    // ENEMY_SQUID_1
    image = LoadImage("resources/img/enemies/1_squid/squid_1.png");
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // ENEMY_SQUID_1_T => index 9
    UnloadImage(image);

    // ENEMY_SQUID_2
    image = LoadImage("resources/img/enemies/1_squid/squid_2.png");
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // ENEMY_SQUID_2_T => index 10
    UnloadImage(image);

    // ENEMY_CRAB_1
    image = LoadImage("resources/img/enemies/2_crab/crab_1.png");
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // ENEMY_CRAB_1_T => index 11
    UnloadImage(image);

    // ENEMY_CRAB_2
    image = LoadImage("resources/img/enemies/2_crab/crab_2.png");
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // ENEMY_CRAB_2_T => index 12
    UnloadImage(image);

    // ENEMY_OCTOPUS_1
    image = LoadImage("resources/img/enemies/3_octopus/octopus_1.png");
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // ENEMY_OCTOPUS_1_T => index 13
    UnloadImage(image);

    // ENEMY_OCTOPUS_2
    image = LoadImage("resources/img/enemies/3_octopus/octopus_2.png");
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // ENEMY_OCTOPUS_2_T => index 14
    UnloadImage(image);

    // EMENY_BULLET_1
    image = LoadImage("resources/img/enemies/bullet/enemy_bullet_1.png");
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // ENEMY_BULLET_1_T => index 15
    UnloadImage(image);

    // EMENY_BULLET_2
    image = LoadImage("resources/img/enemies/bullet/enemy_bullet_2.png");
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // ENEMY_BULLET_2_T => index 16
    UnloadImage(image);

    // EMENY_BULLET_3
    image = LoadImage("resources/img/enemies/bullet/enemy_bullet_3.png");
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // ENEMY_BULLET_3_T => index 17
    UnloadImage(image);

    // EMENY_BULLET_4
    image = LoadImage("resources/img/enemies/bullet/enemy_bullet_4.png");
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // ENEMY_BULLET_4_T => index 18
    UnloadImage(image);

    // EMENY_BULLET_EXPLODING
    image = LoadImage("resources/img/enemies/bullet/enemy_bullet_exploding.png");
    texture = LoadTextureFromImage(image);
    allTexture.push_back(texture);                                                      // ENEMY_BULLET_EXPLODING_T => index 19
    UnloadImage(image);

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
        case GAMEPLAY: InitGameplayScreen(enemies); break;
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
                case GAMEPLAY: InitGameplayScreen(enemies); break;
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
                UpdateGameplayScreen(player, enemyBullets, playerBullets, enemies);

                if (FinishGameplayScreen() == 1) TransitionToScreen(ENDING);
                else if (FinishGameplayScreen() == 2) TransitionToScreen(OPTIONS);

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

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(RAYWHITE);

        switch(currentScreen)
        {
            case LOGO: DrawLogoScreen(); break;
            case TITLE: DrawTitleScreen(); break;
            case OPTIONS: DrawOptionsScreen(); break;
            case GAMEPLAY: DrawGameplayScreen(player, enemyBullets, playerBullets, enemies); break;
            case ENDING: DrawEndingScreen(); break;
            default: break;
        }

        // Draw full screen rectangle in front of everything
        if (onTransition) DrawTransition();

    EndDrawing();
}
