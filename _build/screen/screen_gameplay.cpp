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
int SPEED;
int ROWS_SPEED;
char toastMsg[32];

// Enemies AI
std::array<int, 55> aliveEnemiesMatrix;
std::array<int, 11> enemiesColumnsDeathCounter;
int isAttackerSelected = false;                                               // Allow to AI to select only one enemy per time

// State flags
bool musicState = true;
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

//----------------------------------------------------------------------------------
// __debug__functions__
//----------------------------------------------------------------------------------
void debug_printAlEnMx() {
    printf("__debug__Alive_Enemies_Matrix__\n");
    for (int i = 0; i < 5; i++) {
        printf("%d  %d  %d  %d  %d  %d  %d  %d  %d  %d  %d\n", aliveEnemiesMatrix[i + 0], aliveEnemiesMatrix[i + 5], aliveEnemiesMatrix[i + 10],
            aliveEnemiesMatrix[i + 15], aliveEnemiesMatrix[i + 20], aliveEnemiesMatrix[i + 25], aliveEnemiesMatrix[i + 30], aliveEnemiesMatrix[i + 35],
            aliveEnemiesMatrix[i + 40], aliveEnemiesMatrix[i + 45], aliveEnemiesMatrix[i + 50]);
    }
    printf("\n");
}

//----------------------------------------------------------------------------------
// Player-related functions (player-managing)
//----------------------------------------------------------------------------------

// Determine column status of actually death enemies
void updateEnemiesColumnsDeathCounter(std::vector<Enemy>& enemies) {
    for (int x = 0; x < 11; x++) {
        
        int y = 4;

        while (y >= 0) {
            if(aliveEnemiesMatrix[x * 5 + y] == 0)
                enemiesColumnsDeathCounter[x]++;
            y--;
        }
    }
}

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

                    aliveEnemiesMatrix[((enemy->gridX - 1) * 5 + enemy->gridY)-1] = 0;
                    updateEnemiesColumnsDeathCounter(enemies);

                    printf("___HIT_____enemy_<x:%d><y:%d>__________\n", enemy->gridX, enemy->gridY);
                    debug_printAlEnMx();

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

/*
// Real-time scheduling of an enemy that should shot to the player 
void AI(float playerX, std::vector<Enemy>& enemies) {

    for (auto enemy = begin(enemies); enemy != end(enemies); enemy++) { // enemy++ da vedere...

        // We are acceptable near to the player => check the column and SHOT
        if (playerX <= enemy->position.x + 15.0 && playerX >= enemy->position.x - 15.0) {

            int cX = enemy->gridX;
            int cY = enemy->gridY;
            int i = 4;

            if (cX * cY == 55)
                i = 0;
            else if (cX * cY == 44)
                i = 1;
            else if (cX * cY == 33)
                i = 2;
            else if (cX * cY == 22)
                i = 3;

            // Going to the last available enemy of the column
            while (i > 0 && aliveEnemiesMatrix[(cX - 1) * 5 + (cY - 1) + i] == 0)
                i--;

            if (!enemyAI_flag) {
                (enemy += i)->AI_target = true;
                enemyAI_flag = true;
            }

            printf("___debug__<p.x:%d><t.x:%d>\n", (int)playerX, (int)enemy->position.x);
            return;
        }

        enemy->AI_target = false;
    }

    enemyAI_flag = false;
}*/



/*
// Real-time scheduling of an enemy that should shot to the player 
void AI_2(float playerX, std::vector<Enemy>& enemies) {

    if (checkAttackers(playerX, enemies)) {

    }

    int cursor = (enemies.size() - 1) / 2;

    float fixedEn = enemies[cursor].position.x + enemies[cursor].enemy_T1.width / 2;

    int direction = playerX < fixedEn - 15.0 ? -1 : +1;

    while (cursor >= 0 && cursor <= 54) {

        
        fixedEn = enemies[cursor].position.x + enemies[cursor].enemy_T1.width / 2;

        // We are acceptable near to the player => check the column and SHOT
        if (playerX <= fixedEn + 15.0 && playerX >= fixedEn - 15.0) {

            if (!enemyAI_flag) {
                enemies[cursor].AI_target = true;
                enemyAI_flag = true;
            }

            printf("___debug__<p.x:%d><t.x:%d>\n", (int)playerX, (int)fixedEn);
            return;
        }
        else {
            enemies[cursor].AI_target = false;
            cursor += direction;
        }
            

        

    }

    enemyAI_flag = false;
}*/


// Real-time scheduling of an enemy that should shot to the player 
void AI_3(float playerX, std::vector<Enemy>& enemies) {
        
    for (auto enemy = begin(enemies); enemy != end(enemies); enemy++) {

        int x = enemy->gridX - 1;
        int offset = 4 - enemiesColumnsDeathCounter[x];

        enemy += offset;
        float fixedEn = enemy->position.x + enemy->enemy_T1.width / 2;

        // Player is inside range of the current enemy => SHOT
        if (playerX <= fixedEn + 15.0 && playerX >= fixedEn - 15.0) {

            if (!isAttackerSelected) {
                enemy->AI_target = true;
                isAttackerSelected = true;
            }

            printf("___debug__<p.x:%d><t.x:%d>\n", (int)playerX, (int)fixedEn);
            return;
        }

        enemy->AI_target = false;

    }

    isAttackerSelected = false;
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

    std::sprintf(toastMsg, " ");

    finishScreen = 0;
    aliveEnemiesMatrix.fill(1);
    SPEED = 50;
    ROWS_SPEED = 8;
    
    currRowEnemies = 1;
    musicState = true;
    enemyShouldMove = false;
    state = 0;
    textState = 0;
    textFramesCounter = 0;
    framesCounter = 0;

    enemiesColumnsDeathCounter.fill(0);
    isAttackerSelected = false;
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
        if (musicState) {
            PauseMusicStream(music);
            textState = 1;
        }
        else {
            ResumeMusicStream(music);
            textState = 4;
        }

        PlaySound(fxCoin);
        musicState = !musicState;
    }

    // Manually increase/decrease enemies movement speed
    if (IsKeyPressed(KEY_UP) && SPEED > 1)
    {
        if (SPEED == 50)
            SPEED = 7;
        else {
            SPEED = 1;
            ROWS_SPEED = 1;
        }

        textState = 2;
        PlaySound(fxCoin);
    }
    if (IsKeyPressed(KEY_DOWN) && SPEED != 50)
    {
        textState = 3;
        SPEED = 50;
        ROWS_SPEED = 8;
        PlaySound(fxCoin);
    }

    playerUpdateManager(player, playerBullets, enemyBullets, enemies, eny);
    AI_3(player->position.x + player->player_T.width / 2, enemies);

    // Retractable text managment
    if (textState > 0)
        textFramesCounter++;
    if (textFramesCounter == 100) {
        textState = false;
        textFramesCounter = 0;
    }
    
    // Every SPEED frames all enemies are allowed to move by 1-step
    if (framesCounter % SPEED == 0)
        enemyShouldMove = true;

    // 1-step movement of all enemies is composed by 1 row-per-row movement, that appen every 8 frames
    if (enemyShouldMove && framesCounter % ROWS_SPEED == 0) {
        currRowEnemies = gradualEnemiesMove(enemies, currRowEnemies);

        // If all enemies actually moved then restore the counters
        if (currRowEnemies == 6) {
            framesCounter = 0;
            currRowEnemies = 1;
            enemyShouldMove = false;
        }
    }

    framesCounter++;
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

    // ToastMsg cases based on keyboard events
    switch (textState) {
        int aux;

        case 1:
            aux = MeasureText("music paused", 18);
            DrawText("music paused", GetScreenWidth() / 2 - aux / 2, GetScreenHeight() * 0.85, 18, MAROON);
            break;

        case 2:
            std::sprintf(toastMsg, "speed:\t%d\nrows_speed:\t%d", SPEED, ROWS_SPEED);
            aux = MeasureText(toastMsg, 18);
            DrawText(toastMsg, GetScreenWidth() / 2 - aux / 2, GetScreenHeight() * 0.85, 18, MAROON);
            break;

        case 3:
            std::sprintf(toastMsg, "speed:\t%d\nrows_speed:\t%d", SPEED, ROWS_SPEED);
            aux = MeasureText(toastMsg, 18);
            DrawText(toastMsg, GetScreenWidth() / 2 - aux / 2, GetScreenHeight() * 0.85, 18, MAROON);
            break;

        case 4:
            aux = MeasureText("music resumed", 18);
            DrawText("music resumed", GetScreenWidth() / 2 - aux / 2, GetScreenHeight() * 0.85, 18, MAROON);
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