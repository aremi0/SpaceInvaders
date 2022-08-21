/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
* 
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"
#include <iostream>

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

// Animation
std::vector<Bullet> playerBulletsExplAnim; // Contenitore dei proiettili che stanno esplodendo
int state = 0;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    state = 0;
}

//----------------------------------------------------------------------------------
// Update player related objects before drawing
//----------------------------------------------------------------------------------
void playerUpdateManager(Player* player, std::vector<Bullet>& playerBullets, std::vector<Bullet>& enemyBullets) {

    // Movement-handler with limits checker
    //----------------------------------------------------------------------------------
    if (IsKeyDown(KEY_A) && player->position.x >= 0)
        player->move(Movement::LEFT, GetFrameTime());
    if (IsKeyDown(KEY_D) && player->position.x + player->player_T.width <= GetScreenWidth())
        player->move(Movement::RIGHT, GetFrameTime());

    // Se premo SPAZIO ed il player ha ancora dei colpi disponibili... Creo ed inserisco a schermo un nuovo bullet
    if (IsKeyPressed(KEY_SPACE) && (playerBullets.size() < MAX_PLAYER_BULLETS)) {

        playerBullets.push_back(Bullet(allTexture.at(TextureIndexes::PLAYER_BULLET_T), allTexture.at(TextureIndexes::PLAYER_BULLET_EXPLODING_T),
            Position{ player->position.x + (player->player_T.width / 2), player->position.y }, BulletType::PLAYER_BULLET));

        PlaySound(fxBulletShot);
    }
    //----------------------------------------------------------------------------------

    // Animazione proiettili e loro esplosione...
    //----------------------------------------------------------------------------------
    
    // FLOOR-Explosion Animation
    if (!playerBulletsExplAnim.empty()) {
        for (auto bullet = begin(playerBulletsExplAnim); bullet != end(playerBulletsExplAnim); ) {

//printf("_debug___2_Bullet[%d]__<x:%d><y:%d>\n", bullet->explodingAnimFramesCounter, (int)bullet->position.x, (int)bullet->position.y);
            bullet->explodingAnimFramesCounter--; // Durata dell'esplosione

            if (bullet->explodingAnimFramesCounter <= 0)
                bullet = playerBulletsExplAnim.erase(bullet); // Se l'esplosione è terminata elimino l'oggetto
            else
                bullet++;
        }
    }

    // UP-Movement Animation
    if (!playerBullets.empty()) {
        for (auto bullet = begin(playerBullets); bullet != end(playerBullets); ) {
            // Update posizione bullet
            bullet->move(Movement::UP, GetFrameTime());

// printf("_debug___2_Bullet[%d]__<x:%d><y:%d>\n", bullet->type, (int)bullet->position.x, (int)bullet->position.y);

            // Se il proiettile ha raggiunto il soffito viene inserito tra gli oggetti "proiettili exploding" e rimosso dai proiettili sparati dal player
            if (bullet->position.y <= -10) {
                bullet->position.y = 0;
                playerBulletsExplAnim.push_back(Bullet(allTexture.at(TextureIndexes::PLAYER_BULLET_T), allTexture.at(TextureIndexes::PLAYER_BULLET_EXPLODING_T),
                                                bullet->position, bullet->type, true));
                
                bullet = playerBullets.erase(bullet); // Elimino il bullet che sta esplodendo
            }
            else
                bullet++;
        }
    }
    //----------------------------------------------------------------------------------
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(Player* player, std::vector<Bullet>& playerBullets, std::vector<Bullet>& enemyBullets)
{
    // TODO: Update GAMEPLAY screen variables here!

    // Animation
    if (state == 0) {
        framesCounter++;

        if (framesCounter == 20) {
            state = 1;
            framesCounter = 0;
        }
    }
    else if (state == 1) {
        state = 0;
        framesCounter = 0;
    }

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }

    playerUpdateManager(player, playerBullets, enemyBullets);
}

//----------------------------------------------------------------------------------
// Draw player related objectes
//----------------------------------------------------------------------------------
void playerDrawManager(Player* player, std::vector<Bullet>& playerBullets) {

    // Disegno i proiettili in movimento non ancora esplosi
    if (!playerBullets.empty()) {
        for (auto bullet = begin(playerBullets); bullet != end(playerBullets); bullet++) {
            bullet->draw();
        }
    }

    // Disegno/Animo le esplosioni dei proiettili
    if (!playerBulletsExplAnim.empty()) {
        for (auto bullet = begin(playerBulletsExplAnim); bullet != end(playerBulletsExplAnim); bullet++) {
            bullet->draw();
        }
    }

    player->draw();
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(Player* player, std::vector<Bullet>& playerBullets, std::vector<Bullet>& enemyBullets)
{
    // TODO: Draw GAMEPLAY screen here!

    ClearBackground(RAYWHITE);
    DrawTexture(allTexture.at(TextureIndexes::BACKGROUND_T), 0, 0, WHITE);

    playerDrawManager(player, playerBullets);

    DrawTextEx(font, "GAMEPLAY SCREEN", Vector2{ 20, 10 }, font.baseSize*1.0f, 4, Color {190, 33, 55, 100});
    DrawText("Movement: \tW - A\nShot: \tSPACE", 25, 30, 18, MAROON);
    DrawFPS(GetScreenWidth() - 100, 10);
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}