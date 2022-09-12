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
#include <random>

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int finishScreen = 0;
int SPEED;                                                                    // Speed of enemies's grid movement
int ROWS_SPEED;                                                               // Speed of enemies's grid rows
char toastMsg[32];

int playerBulletsCounter;
int enemiesBulletsCounter;

// Enemies AI
std::array<int, 55> aliveEnemiesMatrix;                                       // 
std::array<int, 11> enemiesColumnsDeathCounter;                               // Determine column status of actually death enemies
int isAttackerSelected = false;                                               // Allows AI to select only one enemy per time
Enemy* attacker;

// State flags
bool musicState = true;
int state = 0;
bool enemyShouldMove = false;
int textState = 0;

// Animation
int currRowEnemies = 1;
Movement enemyCurrentDirection;
std::vector<Bullet> explodingBulletsAnim;                          // Container of player's exploding bullets
std::vector<Enemy> explodingEnemiesAnim;                                   // Container of exploding enemies

// Animation frames counter
int framesCounter = 0;
int textFramesCounter = 0;

//----------------------------------------------------------------------------------
//      __DEBUG
//----------------------------------------------------------------------------------
// UNDONE to delete in future
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
//      UPDATES
//----------------------------------------------------------------------------------

void keyboardEventsHandler(Player* player, std::vector<Bullet>& playerBullets, std::vector<EnemyBullet>& enemiesBullets) {

    // Press ENTER or tap to change to ENDING screen
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

    // Press M to pause/resume background music
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

    // Press UP/DOWN to increase/decrease enemies movement speed
    if (IsKeyPressed(KEY_UP) && SPEED > 1)
    {
        if (SPEED == 50) {
            SPEED = 7;
            framesCounter = 0;
        }
        else {
            SPEED = 1;
            ROWS_SPEED = 1;
            framesCounter = 0;
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

    // Press TAB to let enemy_AI shot a bullet
    if (IsKeyPressed(KEY_TAB) && isAttackerSelected)
    {
        // HACK when AI will be more sofisticated will be selected enemies bullet type based on the target (player => faster || bunker => powerful)
        //----------------------------------------------------------------------------------
        // Randomly selecting a enemy bullet type
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> genRand(BulletType::SLOWER_BULLET, BulletType::POWERFUL_BULLET);

        BulletType r = static_cast<BulletType> (genRand(rng));
        int index = -1;

        if (r == BulletType::SLOWER_BULLET)
            index = TextureIndexes::ENEMY_SLOW_BULLET_1_T;
        else if (r == BulletType::FASTER_BULLET)
            index = TextureIndexes::ENEMY_FASTER_BULLET_1_T;
        else
            index = TextureIndexes::ENEMY_POWERFUL_BULLET_1_T;
        //----------------------------------------------------------------------------------

        enemiesBullets.push_back(EnemyBullet(allTexture.at(index), allTexture.at(index + 1), allTexture.at(index + 2),
            allTexture.at(index + 3), allTexture.at(TextureIndexes::ENEMY_BULLET_EXPLODING_T),
            Position{ attacker->position.x + (attacker->enemy_T1.width / 2), attacker->position.y }, r));

        PlaySound(fxCoin);
    }

    // Player movement handler with screen limits checker
    if (IsKeyDown(KEY_A) && player->position.x >= 0)
        player->move(Movement::LEFT, GetFrameTime());
    if (IsKeyDown(KEY_D) && player->position.x + player->player_T.width <= GetScreenWidth())
        player->move(Movement::RIGHT, GetFrameTime());

    // Press SPACE to let player shot a bullet
    if (IsKeyPressed(KEY_SPACE) && (playerBullets.size() < MAX_PLAYER_BULLETS)) {

        playerBullets.push_back(Bullet(allTexture.at(TextureIndexes::PLAYER_BULLET_T), allTexture.at(TextureIndexes::PLAYER_BULLET_EXPLODING_T),
            Position{ player->position.x + (player->player_T.width / 2), player->position.y }, BulletType::PLAYER_BULLET));

        PlaySound(fxBulletShot);
    }
}

void bulletsMovementsHandler(std::vector<Bullet>& playerBullets, std::vector<EnemyBullet>& enemiesBullets) {

    // Player-Bullets movement animation
    if (!playerBullets.empty()) {
        for (auto& bul = begin(playerBullets); bul != end(playerBullets); ) {

            bul->move(GetFrameTime()); // Update bullet position

            if (bul->position.y <= -10) { // If the current player bullet reach the screen roof => StartExploding()

                bul->position.y = 0;
                explodingBulletsAnim.push_back(*bul->StartExploding());
                bul = playerBullets.erase(bul);
            }
            else
                bul++;
        }
    }

    // Enemies-Bullets movement animation
    if (!enemiesBullets.empty()) {
        for (auto& bul = begin(enemiesBullets); bul != end(enemiesBullets); ) {

            bul->move(GetFrameTime()); // Update bullet position

            printf("___debug__<bul.type:%d><bul.y:%d>\n", bul->type, (int)bul->position.y);

            if (bul->position.y + bul->eBullet_T1.height >= GetScreenHeight()) { // If the current enemy bullet reach the screen floor => StartExploding()
                bul->position.y = GetScreenHeight() - bul->bulletExplodind_T.height;
                explodingBulletsAnim.push_back(*bul->StartExploding());
                bul = enemiesBullets.erase(bul);
            }
            else
                bul++;
        }
    }
}

void explodingAnimHandler() {

    // FLOOR & ROOF bullets exploding animation
    if (!explodingBulletsAnim.empty()) {
        for (auto bul = begin(explodingBulletsAnim); bul != end(explodingBulletsAnim); ) {

            printf("_|||||__debug__<bul.type:%d><bul.y:%d>\n", bul->type, (int)bul->position.y);

            bul->explodingAnimFramesCounter--; // Explosion duration

            if (bul->explodingAnimFramesCounter <= 0)
                bul = explodingBulletsAnim.erase(bul); // If explosion terminated => delete object
            else
                bul++;
        }
    }

    // HITTED Enemies exploding animation
    if (!explodingEnemiesAnim.empty()) {
        for (auto& enemy = begin(explodingEnemiesAnim); enemy != end(explodingEnemiesAnim); ) {
            enemy->explodingAnimFramesCounter--;

            if (enemy->explodingAnimFramesCounter <= 0)
                enemy = explodingEnemiesAnim.erase(enemy);
            else
                enemy++;
        }
    }
}

// Check collisions of player's bullets on enemies grid
void enemies_CollisionDetector(std::vector<Bullet>& playerBullets, std::vector<Enemy>& enemies) {

    if (!playerBullets.empty() && !enemies.empty()) {
        for (auto bul = begin(playerBullets); bul != end(playerBullets); bul++) {
            for (auto enemy = begin(enemies); enemy != end(enemies); ) {
                // For each shotted player's bullet check collision with each enemy...
                if (CheckCollisionRecs(Rectangle{ enemy->position.x, enemy->position.y, enemy->enemy_T1.width * 1.0f, enemy->enemy_T1.height * 1.0f },
                    Rectangle{ bul->position.x, bul->position.y, bul->bullet_T.width * 1.0f, bul->bullet_T.height * 1.0f })) {

                    // Updates enemies AI structure
                    aliveEnemiesMatrix[((enemy->gridX - 1) * 5 + enemy->gridY) - 1] = 0;
                    enemiesColumnsDeathCounter[enemy->gridX - 1]++;
                    //----------------------------------------------------------------------------------

                    printf("___HIT_____enemy_<x:%d><y:%d>__________\n", enemy->gridX, enemy->gridY);
                    debug_printAlEnMx();

                    PlaySound(fxEnemyExplosion);
                    explodingEnemiesAnim.push_back(*enemy->StartExploding());
                    enemy = enemies.erase(enemy);
                    bul->position.x = DEBUG_X;
                }
                else
                    enemy++;
            }
        }
    }
}

// Handle all updates before drawing
void updateManager(Player* player, std::vector<Bullet>& playerBullets, std::vector<EnemyBullet>& enemiesBullets, std::vector<Enemy>& enemies) {

    keyboardEventsHandler(player, playerBullets, enemiesBullets);
    explodingAnimHandler();
    bulletsMovementsHandler(playerBullets, enemiesBullets);
    enemies_CollisionDetector(playerBullets, enemies);
}

//----------------------------------------------------------------------------------
//      DRAWS
//----------------------------------------------------------------------------------

// Manage all text to draw
void textHandler() {

    DrawTextEx(font, "GAMEPLAY SCREEN", Vector2{ 20, 10 }, font.baseSize * 1.0f, 4, Color{ 190, 33, 55, 100 });
    //DrawText("Movement: \tW - A\nShot: \tSPACE", 25, 30, 18, MAROON);
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
    }
}

// Handle all elements to draw
void drawManager(Player* player, std::vector<Bullet>& playerBullets, std::vector<Enemy>& enemies, std::vector<EnemyBullet>& enemiesBullets) {

    textHandler();

    // Draw player's shotted bullets
    if (!playerBullets.empty()) {
        for (auto bullet = begin(playerBullets); bullet != end(playerBullets); bullet++) {
            bullet->draw();
        }
    }

    // Draw exploding bullets animation
    if (!explodingBulletsAnim.empty()) {
        for (auto bullet = begin(explodingBulletsAnim); bullet != end(explodingBulletsAnim); bullet++) {
            bullet->draw();
        }
    }

    player->draw(); // Draw the player

    // Draw enemies's shotted bullets
    if (!enemiesBullets.empty()) {
        for (auto bullet = begin(enemiesBullets); bullet != end(enemiesBullets); bullet++) {
            bullet->draw();
        }
    }

    // Draw the enemies
    for (auto& enemy : enemies) {
        enemy.draw();
    }

    // Draw esploding enemies animation
    for (auto& explodingEnemy : explodingEnemiesAnim) {
        explodingEnemy.draw();
    }
}

// Allow enemies to moves gradually row-per-row
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

// Real-time scheduling of an enemy that should shot to the player 
Enemy* enemiesAI_3(float playerX, std::vector<Enemy>& enemies) {

    // HACK enemiesAI
    /**     DESCRIPTION of the current alghorithm + POSSIBLE UPGRADE
    * 
    * Flows the enemies vector offset-per-offset; where offset is calculated as the index of the most exposed enemy (exposed row)
    * of the current column thanks to enemiesColumnsDeathCounter[].
    * In total will scan each frame at most 11 enemies. (Less if a column is totally epmty -killed-)
    * 
    * This AI can be improved (?) in this way:
    * - We know player.x
    * - We know distance from each column (enemy margin = this->enemy_T1.width + (GetScreenWidth() * 0.075) * this->gridX)
    * 
    * Starting from the "enemy += offset" we can compare current enemy.x with player.x and:
    *       
    *       int columnOffset = 11 - enemy.gridX;
    * 
    *       if (player.x <= columnOffset * enemy.margin + 15 && player.x >= enemy.margin * enemy.gridX - 15) // Player is inside the enemies grid
    *       do  //player is on the right...
    *           int playerColumn = player.x / enemy.margin                    // This is the index 0 to 10
    *           int totalJump = (playerColumn+1 - enemy.gridX+1) * 5          // Total enemies to jump (death and alive) if positive => go to right | else go to left
    *           
    *           if(aliveEnemiesMatrix[playerColumn * 5 + 1] == 1)             // Check if at least the enemy in the least exposed row (1-row) of the player column is alive     
    *           do
    *               int specificJump = totalJump;
    *               for(int i = enemy.gridX+1; i < playerColumn+1; i++)
    *                  specificJump -= enemiesColumnsDeathCounter[i]          // specificJump is equal to the total of enemies to jump - the number of deaths till the position
    * 
    */

    for (auto enemy = begin(enemies); enemy != end(enemies); enemy++) {

        int x = enemy->gridX - 1;
        int offset = 4 - enemiesColumnsDeathCounter[x];

        enemy += offset;
        float enemyX = enemy->position.x + enemy->enemy_T1.width / 2;

        // Player is inside range of the current enemy => SHOT
        if (playerX <= enemyX + 15.0 && playerX >= enemyX - 15.0) {

            if (!isAttackerSelected) {
                enemy->AI_target = true;
                isAttackerSelected = true;
            }

            //printf("___debug__<p.x:%d><t.x:%d>\n", (int)playerX, (int)enemyX);
            return enemy._Ptr;
        }

        enemy->AI_target = false;

    }

    isAttackerSelected = false;
}

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(std::vector<Enemy>& enemies)
{
    // Initialize GAMEPLAY screen variables here!
    if(enemies.front().direction == -1)
        enemyCurrentDirection = Movement::LEFT;
    else
        enemyCurrentDirection = Movement::RIGHT;

    std::sprintf(toastMsg, " ");

    finishScreen = 0;
    aliveEnemiesMatrix.fill(1);
    SPEED = 50;
    ROWS_SPEED = 8;
    playerBulletsCounter = 0;
    enemiesBulletsCounter = 0;
    
    currRowEnemies = 1;
    musicState = true;
    enemyShouldMove = false;
    state = 0;
    textState = 0;
    textFramesCounter = 0;
    framesCounter = 0;

    enemiesColumnsDeathCounter.fill(0);
    isAttackerSelected = false;
    attacker = nullptr;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(Player* player, std::vector<Bullet>& playerBullets, std::vector<EnemyBullet>& enemiesBullets, std::vector<Enemy>& enemies)
{

    updateManager(player, playerBullets, enemiesBullets, enemies);
    attacker = enemiesAI_3(player->position.x + player->player_T.width / 2, enemies);

    // Retractable text managment
    if (textState > 0)
        textFramesCounter++;
    if (textFramesCounter == 100) {
        textState = false;
        textFramesCounter = 0;
    }
    
    // Every SPEED frames all enemies are allowed to move by 1-step
    if (framesCounter == SPEED) {
        enemyShouldMove = true;
        PlaySound(fxEnemyMove);
    }

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
void DrawGameplayScreen(Player* player, std::vector<Bullet>& playerBullets, std::vector<EnemyBullet>& enemiesBullets, std::vector<Enemy>& enemies)
{

    ClearBackground(RAYWHITE);
    DrawTexture(allTexture.at(TextureIndexes::BACKGROUND_T), 0, 0, WHITE);  // Draw background

    drawManager(player, playerBullets, enemies, enemiesBullets);            // Draw gameplay's elements
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}