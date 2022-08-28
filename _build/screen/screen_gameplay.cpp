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
int state = 0;
bool writeState = 0;

// Animation
Movement enemyCurrentDirection = Movement::RIGHT;
std::vector<Bullet> playerBulletsExplAnim; // Contenitore dei proiettili che stanno esplodendo

//----------------------------------------------------------------------------------
// Player-related functions (player-managing)
//----------------------------------------------------------------------------------

// Update player related objects before drawing
void playerUpdateManager(Player* player, std::vector<Bullet>& playerBullets, std::vector<Bullet>& enemyBullets) {

    // Movement-handler with screen limits checker
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
}

// Draw player related objectes
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

//----------------------------------------------------------------------------------
// Enemies-related functions (enemies-managing)
//----------------------------------------------------------------------------------

// Update enemies related objects before drawing
void enemiesUpdateManager(std::vector<Enemy>& enemies, std::vector<Bullet>& enemyBullets) {

    // Screen limits checker
    if (enemyCurrentDirection == Movement::LEFT) {

        if (enemies.begin()->position.x <= GetScreenWidth() * 0.01) {
            for (auto& enemy : enemies) {
                enemy.move(GetFrameTime(), true);
            }

            enemyCurrentDirection = Movement::RIGHT;
        }
        else {
            for (auto& enemy : enemies) {
                enemy.move(GetFrameTime());
                printf("_debug__enemy_[%d][%d]___<x:%d><y:%d>\n", enemy.gridX, enemy.gridY, (int)enemy.position.x, (int)enemy.position.y);
            }
        }
    }
    else if (enemyCurrentDirection == Movement::RIGHT) {
        
        if (enemies.back().position.x + enemies.back().enemy_T1.width >= GetScreenWidth() - (GetScreenWidth() * 0.01)) {
            for (auto& enemy : enemies)
                enemy.move(GetFrameTime(), true);

            enemyCurrentDirection = Movement::LEFT;
        }
        else {
            for (auto& enemy : enemies) {
                enemy.move(GetFrameTime());
                printf("_debug__enemy_[%d][%d]___<x:%d><y:%d>\n", enemy.gridX, enemy.gridY, (int)enemy.position.x, (int)enemy.position.y);
            }
        }
    }

    printf("\n");
}

// Draw enemies related objectes
void enemiesDrawManager(std::vector<Enemy>& enemies, std::vector<Bullet>& enemyBullets) {


    for (auto& enemy : enemies) {
        enemy.draw();
    }
}

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    enemyCurrentDirection = Movement::RIGHT;

    framesCounter = 0;
    finishScreen = 0;
    state = 0;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(Player* player, std::vector<Bullet>& playerBullets, std::vector<Bullet>& enemyBullets, std::vector<Enemy>& enemies)
{
    // TODO: Update GAMEPLAY screen variables here!

    // DIFFICULTY level 
    if (state <= 4) { // slow
        framesCounter++;

        if (framesCounter == 70) {
            framesCounter = 0;
            state++;
            enemiesUpdateManager(enemies, enemyBullets);
        }
    }
    else if (state <= 80) { // faster
        framesCounter++;

        if (framesCounter % 5 == 0) {
            framesCounter = 0;
            state++;
            enemiesUpdateManager(enemies, enemyBullets);
        }

        if (state <= 30) {
            writeState = true;
        }
        else
            writeState = false;
    }

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }

    playerUpdateManager(player, playerBullets, enemyBullets);
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(Player* player, std::vector<Bullet>& playerBullets, std::vector<Bullet>& enemyBullets, std::vector<Enemy>& enemies)
{
    // TODO: Draw GAMEPLAY screen here!

    ClearBackground(RAYWHITE);
    DrawTexture(allTexture.at(TextureIndexes::BACKGROUND_T), 0, 0, WHITE);

    playerDrawManager(player, playerBullets);
    enemiesDrawManager(enemies, enemyBullets);

    DrawTextEx(font, "GAMEPLAY SCREEN", Vector2{ 20, 10 }, font.baseSize*1.0f, 4, Color {190, 33, 55, 100});
    DrawText("Movement: \tW - A\nShot: \tSPACE", 25, 30, 18, MAROON);
    DrawFPS(GetScreenWidth() - 100, 10);

    if (writeState) {
        int aux = MeasureText("state changed => faster", 18);
        DrawText("state changed => faster", GetScreenWidth() / 2 - aux / 2, GetScreenHeight() / 2, 18, MAROON);
    }
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