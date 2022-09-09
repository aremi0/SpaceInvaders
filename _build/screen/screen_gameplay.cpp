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
static int finishScreen = 0;
int SPEED = 50;
std::array<int, 55> enemiesMatrix;

// State flags
int state = 0;
bool enemyShouldMove = false;
int textState = 0;

// Animation
int currRowEnemies = 1;
Movement enemyCurrentDirection;
std::vector<Bullet> playerBulletsExplAnim;                          // Container of player's exploding bullets
std::vector<Enemy> enemyExplAnim;                                   // Container of exploding enemies

// Animation frames counter
int framesCounter = 0;
int textFramesCounter = 0;
int rowMovEnemyFramesCounter = 0;

//----------------------------------------------------------------------------------
// Player-related functions (player-managing)
//----------------------------------------------------------------------------------

// Update player related objects before drawing
void playerUpdateManager(Player* player, std::vector<Bullet>& playerBullets, std::vector<Bullet>& enemyBullets, std::vector<Enemy>& enemies, std::array<Enemy*, 55>& eny) {

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

// DEPRECATED BY gradualEnemiesMove()
/*// Update enemies related objects before drawing
void enemiesUpdateManager(std::vector<Enemy>& enemies, std::vector<Bullet>& enemyBullets, std::array<Enemy*, 55>& eny) {

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
*/

int gradualEnemiesMove(std::vector<Enemy>& enemies, int row) {
    
    // Gradual enemies movement with screen limits checker
    //----------------------------------------------------------------------------------
    if (!enemies.empty()) {

        if (enemyCurrentDirection == Movement::LEFT) {                                      // Going to LEFT and...
            if (row == 1 && enemies.begin()->position.x <= SCREEN_WIDTH_MARGIN) {           // ...all 5 rows moved and I'm alredy out of margin...

                for (auto& enemy : enemies) {                                               // ...go down and invert movement direction
                    enemy.move(GetFrameTime(), true);
                }

                enemyCurrentDirection = Movement::RIGHT;
                return 1;                                                                   // Next move will be for row 1
            }
            else {                                                                          // ...else, if i'm not illegal... just move

                for (auto& enemy : enemies) {
                    if(enemy.gridY == row)
                        enemy.move(GetFrameTime());
                }
            }
        }
        else if (enemyCurrentDirection == Movement::RIGHT) {
            if (row == 1 && enemies.rbegin()->position.x + enemies.rbegin()->enemy_T1.width >= GetScreenWidth() - (GetScreenWidth() * 0.01)) {
                for (auto& enemy : enemies)
                    enemy.move(GetFrameTime(), true);

                enemyCurrentDirection = Movement::LEFT;
                return 1;
            }
            else {
                for (auto& enemy : enemies) 
                    if (enemy.gridY == row)
                        enemy.move(GetFrameTime());
            }
        }
    }

    return row + 1;                                                                         // Next move will be on the next row
}

// Draw enemies related objectes
void enemiesDrawManager(std::vector<Enemy>& enemies, std::vector<Bullet>& enemyBullets, std::array<Enemy*, 55>& eny) {

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
    if(enemies.front().direction == -1)
        enemyCurrentDirection = Movement::LEFT;
    else
        enemyCurrentDirection = Movement::RIGHT;

    finishScreen = 0;
    enemiesMatrix.fill(1);
    SPEED = 50;
    
    currRowEnemies = 1;
    enemyShouldMove = false;
    state = 0;
    textState = 0;
    textFramesCounter = 0;
    framesCounter = 0;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(Player* player, std::vector<Bullet>& playerBullets, std::vector<Bullet>& enemyBullets, std::vector<Enemy>& enemies, std::array<Enemy*, 55>& eny)
{
    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }

    // Press P to change to PAUSE screen
    if (IsKeyPressed(KEY_P))
    {
        finishScreen = 2;
        PlaySound(fxCoin);
    }

    // Press M to mute background music
    if (IsKeyPressed(KEY_M))
    {
        textState = 1;
        SetMusicVolume(music, 0.0f);
        PlaySound(fxCoin);
    }

    // Manually increase/decrease enemies movement speed
    if (IsKeyPressed(KEY_UP))
    {
        textState = 2;
        SPEED = 10;
        PlaySound(fxCoin);
    }
    if (IsKeyPressed(KEY_DOWN))
    {
        textState = 3;
        SPEED = 50;
        PlaySound(fxCoin);
    }

    playerUpdateManager(player, playerBullets, enemyBullets, enemies, eny);

    // Retractable text managment
    if (textState > 0)
        textFramesCounter++;
    if (textFramesCounter == 100) {
        textState = false;
        textFramesCounter = 0;
    }
    
    // Each SPEED frames all enemies are allowed to moves
    if (framesCounter % SPEED == 0)
        enemyShouldMove = true;

    // If all enemies are allowed to moves then move them row-per-row every 15 frames
    if (enemyShouldMove && framesCounter % 5 == 0) {
        currRowEnemies = gradualEnemiesMove(enemies, currRowEnemies);

        // If all enemies actually moved then restore the counters
        if (currRowEnemies == 6) {
            framesCounter = 0;
            currRowEnemies = 1;
            enemyShouldMove = false;
        }
    }

    framesCounter++;
    rowMovEnemyFramesCounter++;
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(Player* player, std::vector<Bullet>& playerBullets, std::vector<Bullet>& enemyBullets, std::vector<Enemy>& enemies, std::array<Enemy*, 55>& eny)
{
    // TODO: Draw GAMEPLAY screen here!

    ClearBackground(RAYWHITE);
    DrawTexture(allTexture.at(TextureIndexes::BACKGROUND_T), 0, 0, WHITE);

    playerDrawManager(player, playerBullets);
    enemiesDrawManager(enemies, enemyBullets, eny);

    DrawTextEx(font, "GAMEPLAY SCREEN", Vector2{ 20, 10 }, font.baseSize*1.0f, 4, Color {190, 33, 55, 100});
    DrawText("Movement: \tW - A\nShot: \tSPACE", 25, 30, 18, MAROON);
    DrawFPS(GetScreenWidth() - 100, 10);

    // Text cases based on keyboard events
    switch (textState)
    {
        int aux;

        case 1:
            aux = MeasureText("music muted", 18);
            DrawText("music muted", GetScreenWidth() / 2 - aux / 2, GetScreenHeight() * 0.85, 18, MAROON);
            break;

        case 2:
            aux = MeasureText("speed increased", 18);
            DrawText("speed increased", GetScreenWidth() / 2 - aux / 2, GetScreenHeight() * 0.85, 18, MAROON);
            break;

        case 3:
            aux = MeasureText("speed decreased", 18);
            DrawText("speed decreased", GetScreenWidth() / 2 - aux / 2, GetScreenHeight() * 0.85, 18, MAROON);
            break;

        default:
            break;
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