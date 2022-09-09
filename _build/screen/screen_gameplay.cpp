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
int SPEED = 50;

bool writeState = false;
static int writeFramesCounter = 0;

// Animation
Movement enemyCurrentDirection;
std::vector<Bullet> playerBulletsExplAnim;                          // Container of player's exploding bullets
std::vector<Enemy> enemyExplAnim;                                   // Container of exploding enemies

//----------------------------------------------------------------------------------
// Player-related functions (player-managing)
//----------------------------------------------------------------------------------

// Update player related objects before drawing
void playerUpdateManager(Player* player, std::vector<Bullet>& playerBullets, std::vector<Bullet>& enemyBullets, std::vector<Enemy>& enemies) {

    // Player movement handler with screen limits checker
    //----------------------------------------------------------------------------------
    if (IsKeyDown(KEY_A) && player->position.x >= 0)
        player->move(Movement::LEFT, GetFrameTime());
    if (IsKeyDown(KEY_D) && player->position.x + player->player_T.width <= GetScreenWidth())
        player->move(Movement::RIGHT, GetFrameTime());

    // If SPACE is pressed and player still has shottable bullets... Create and draw new player bullet
    if (IsKeyPressed(KEY_SPACE) && (playerBullets.size() < MAX_PLAYER_BULLETS)) {

        playerBullets.push_back(Bullet(allTexture.at(TextureIndexes::PLAYER_BULLET_T), allTexture.at(TextureIndexes::PLAYER_BULLET_EXPLODING_T),
            Position{ player->position.x + (player->player_T.width / 2), player->position.y }, BulletType::PLAYER_BULLET));

        PlaySound(fxBulletShot);
    }

    // Player's bullets animation & explosion
    //----------------------------------------------------------------------------------
    // FLOOR-explosion animation
    if (!playerBulletsExplAnim.empty()) {
        for (auto bullet = begin(playerBulletsExplAnim); bullet != end(playerBulletsExplAnim); ) {

            bullet->explodingAnimFramesCounter--; // Explosion duration

            if (bullet->explodingAnimFramesCounter <= 0)
                bullet = playerBulletsExplAnim.erase(bullet); // If explosion terminated => delete object
            else
                bullet++;
        }
    }

    // UP-movement animation
    if (!playerBullets.empty()) {
        for (auto bullet = begin(playerBullets); bullet != end(playerBullets); ) {
            
            bullet->move(Movement::UP, GetFrameTime()); // Update bullet position

            if (bullet->position.y <= -10) { // If the current player bullet reach the screen roof => StartExploding()
                
                bullet->position.y = 0;
                playerBulletsExplAnim.push_back(*bullet->StartExploding());
                bullet = playerBullets.erase(bullet); // Elimino il bullet che sta esplodendo
            }
            else
                bullet++;
        }
    }

    // Enemy Collision-detector & eplosion animation
    //----------------------------------------------------------------------------------
    // HIT-explosion animation
    if (!enemyExplAnim.empty()) {
        for (auto& enemy = begin(enemyExplAnim); enemy != end(enemyExplAnim); ) {
            enemy->explodingAnimFramesCounter--;

            if (enemy->explodingAnimFramesCounter <= 0)
                enemy = enemyExplAnim.erase(enemy);
            else
                enemy++;
        }
    }

    // player's bullets vs enemy => Collision-Detector
    if (!playerBullets.empty() && !enemies.empty()) {
        for (auto bullet = begin(playerBullets); bullet != end(playerBullets); bullet++) {
            for (auto enemy = begin(enemies); enemy != end(enemies); ) {
                // For each shotted player's bullet check collision with each enemy...
                if (CheckCollisionRecs(Rectangle{ enemy->position.x, enemy->position.y, enemy->enemy_T1.width * 1.0f, enemy->enemy_T1.height * 1.0f },
                    Rectangle{ bullet->position.x, bullet->position.y, bullet->bullet_T.width * 1.0f, bullet->bullet_T.height * 1.0f })) {

                    printf("___HIT_____enemy_<x:%d><y:%d>__________\n", enemy->gridX, enemy->gridY);
                    PlaySound(fxEnemyExplosion);
                    enemyExplAnim.push_back(*enemy->StartExploding());
                    enemy = enemies.erase(enemy);
                    bullet->position.x = DEBUG_X;
                }
                else
                    enemy++;
            }
        }
    }
}

// Draw player related objectes
void playerDrawManager(Player* player, std::vector<Bullet>& playerBullets) {

    // Draw UP-movement of shotted player's bullets
    if (!playerBullets.empty()) {
        for (auto bullet = begin(playerBullets); bullet != end(playerBullets); bullet++) {
            bullet->draw();
        }
    }

    // Draw player's bullet explosion animation
    if (!playerBulletsExplAnim.empty()) {
        for (auto bullet = begin(playerBulletsExplAnim); bullet != end(playerBulletsExplAnim); bullet++) {
            bullet->draw();
        }
    }

    player->draw(); // Draw the player tank
}

//----------------------------------------------------------------------------------
// Enemies-related functions (enemies-managing)
//----------------------------------------------------------------------------------

// Update enemies related objects before drawing
void enemiesUpdateManager(std::vector<Enemy>& enemies, std::vector<Bullet>& enemyBullets) {

    // Enemies movement with screen limits checker
    //----------------------------------------------------------------------------------
    if (!enemies.empty()) {

        printf("___0-0_enemy____enemy_<x:%d><y:%d>__________\n", (int)enemies.begin()->position.x, (int)enemies.begin()->position.x);

        if (enemyCurrentDirection == Movement::LEFT) { // Going to LEFT and...
            if (enemies.begin()->position.x <= SCREEN_WIDTH_MARGIN) { // ...i'm alredy out of margin...
                for (auto& enemy : enemies) { // ...go down and invert movement direction
                    enemy.move(GetFrameTime(), true);
                }

                enemyCurrentDirection = Movement::RIGHT;
            }
            else { // ...else, if i'm not illegal... just move
                for (auto& enemy : enemies) {
                    enemy.move(GetFrameTime());
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
                }
            }
        }
    }
}

// Draw enemies related objectes
void enemiesDrawManager(std::vector<Enemy>& enemies, std::vector<Bullet>& enemyBullets) {


    for (auto& enemy : enemies) {
        enemy.draw();
    }

    for (auto& explodingEnemy : enemyExplAnim) {
        explodingEnemy.draw();
    }
}

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(std::vector<Enemy>& enemies)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    if(enemies.begin()->direction == -1)
        enemyCurrentDirection = Movement::LEFT;
    else
        enemyCurrentDirection = Movement::RIGHT;

    SPEED = 50;
    framesCounter = 0;
    finishScreen = 0;
    state = 0;

    writeState = false;
    writeFramesCounter = 0;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(Player* player, std::vector<Bullet>& playerBullets, std::vector<Bullet>& enemyBullets, std::vector<Enemy>& enemies)
{
    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }

    // Press M to mute background music
    if (IsKeyPressed(KEY_M))
    {
        SetMusicVolume(music, 0.0f);
        PlaySound(fxCoin);
    }

    // Manually increase/decrease enemies movement speed
    if (IsKeyPressed(KEY_UP))
    {
        writeState = true;
        SPEED = 10;
        PlaySound(fxCoin);
    }
    if (IsKeyPressed(KEY_DOWN))
    {
        writeState = true;
        SPEED = 50;
        PlaySound(fxCoin);
    }

    playerUpdateManager(player, playerBullets, enemyBullets, enemies);

    if (writeState)
        writeFramesCounter++;

    if (writeFramesCounter == 100) {
        writeState = false;
        writeFramesCounter = 0;
    }

    //every SPEED frame a enemies movement
    framesCounter++;
    if (framesCounter >= SPEED) {
        enemiesUpdateManager(enemies, enemyBullets);
        framesCounter = 0;
    }


    // Speed of enemies 
    /*
    if (state <= 4) { 
        framesCounter++;

        if (framesCounter == 70) { // slow
            framesCounter = 0;
            state++;
            enemiesUpdateManager(enemies, enemyBullets);
        }
    }
    else if (state > 4) { 
        framesCounter++;

        if (framesCounter % 5 == 0) { // faster
            framesCounter = 0;
            state++;
            enemiesUpdateManager(enemies, enemyBullets);
        }

        if (state <= 30) {
            writeState = true;
        }
        else
            writeState = false;
    }*/

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
        int aux = MeasureText("speed changed", 18);
        DrawText("speed changed", GetScreenWidth() / 2 - aux / 2, GetScreenHeight() * 0.85, 18, MAROON);
        DrawText("speed changed", GetScreenWidth() / 2 - aux / 2, GetScreenHeight() * 0.85, 18, MAROON);
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