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
#include <string>


#define MOVEMENT_SPEED 50
#define ROW_GRADUAL_SPEED 8


#define AI_REFRESH_RATE 10
#define ENEMIES_MAX_BULLET 3
#define ENEMIES_WAIT_BEFORE_SHOT 50                                           // Set the enemies fire rateo (frames between shots)

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------

// __debug_mode
unsigned debug_mode;
std::array<std::string, 10> debug_text;

// View
static int finishScreen = 0;
int textState = 0;
bool musicState = true;
int framesCounter = 0;
int textFramesCounter = 0;
char toastMsg[128];
//----------------------------------------------------------------------------------
// Gameplay
int SPEED;                                                                    // Speed of enemies's grid movement
int ROWS_SPEED;                                                               // Speed of enemies's grid rows
bool enemyShouldMove = false;
//----------------------------------------------------------------------------------
// Animation
int currRowEnemies = 1;
Movement enemyCurrentDirection;
std::vector<Bullet> explodingBulletsAnim;                                  // Container of player's exploding bullets
std::vector<Enemy> explodingEnemiesAnim;                                   // Container of exploding enemies
//----------------------------------------------------------------------------------
// Enemies
std::array<int, 11> enemiesColumnsDeathCounter;                               // Determine column status of actually death enemies
Enemy* attacker;
int enemyShotWait;
bool enemyHasShot;
bool enemyCanShot;
// predictive_AI
int AI_framesCounter;
float currPl_x;
float nextPl_x;
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
//      __DEBUG
//----------------------------------------------------------------------------------


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
    // TODO pause-screen
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

        if (debug_mode == 2) {
            debug_text[3] = std::to_string(SPEED);
            debug_text[5] = std::to_string(ROWS_SPEED);
        }

        textState = 2;
        PlaySound(fxCoin);
    }
    if (IsKeyPressed(KEY_DOWN) && SPEED != 50)
    {
        textState = 3;
        SPEED = MOVEMENT_SPEED;
        ROWS_SPEED = ROW_GRADUAL_SPEED;

        if (debug_mode == 2) {
            debug_text[3] = std::to_string(SPEED);
            debug_text[5] = std::to_string(ROWS_SPEED);
        }

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

    // Press TAB to enter __debug_mode
    // TODO __debug_mode
    if (IsKeyPressed(KEY_TAB)) {

        switch (debug_mode)
        {
            case 0: {
                debug_mode = 1;
                debug_text.fill("");
                debug_text[0] = "__debug_mode:";
                debug_text[1] = "enemies_AI";
                debug_text[2] = "player is:";
                debug_text[4] = "strategy:";
                debug_text[6]= "target.x:";
                break;
            }

            case 1: {
                debug_mode = 2;
                debug_text.fill("");
                debug_text[0] = "__debug_mode:";
                debug_text[1] = "difficulty";
                debug_text[2] = "mov.speed:";
                debug_text[3] = std::to_string(SPEED);
                debug_text[4] = "row.mov.speed:";
                debug_text[5] = std::to_string(ROWS_SPEED);
                debug_text[6] = "rate.of.fire:";
                
                break;
            }
            case 2: {
                debug_mode = 3;
                debug_text.fill("");
                debug_text[0] = "__debug_mode:";
                debug_text[1] = "bunker_status";
                break;
            }
            case 3: {
                debug_mode = 0;
                debug_text.fill("");
                break;
            }
        }
    }
}

void bulletsMovementsHandler(std::vector<Bullet>& playerBullets, std::vector<EnemyBullet>& enemiesBullets) {

    // Player-Bullets movement animation
    if (!playerBullets.empty()) {
        for (auto& bul = begin(playerBullets); bul != end(playerBullets); ) {

            bul->move(GetFrameTime()); // Update bullet position

            if (bul->position.y <= -10) { // If the current player bullet reach the screen roof => StartExploding()

                bul->position.y = 0;
                bul->exploding = true;
                explodingBulletsAnim.push_back(*bul._Ptr);
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

            //printf("___debug__<bul.type:%d><bul.y:%d>\n", bul->type, (int)bul->position.y);

            if (bul->position.y + bul->eBullet_T1.height >= GetScreenHeight()) { // If the current enemy bullet reach the screen floor => StartExploding()

                bul->position.y = GetScreenHeight() - bul->bulletExplodind_T.height;
                bul->exploding = true;
                explodingBulletsAnim.push_back(*bul._Ptr);
                bul = enemiesBullets.erase(bul);
            }
            else
                bul++;
        }
    }
}

void explodingAnimHandler(Player* player) {

    // FLOOR & ROOF bullets exploding animation
    if (!explodingBulletsAnim.empty()) {
        for (auto& bul = begin(explodingBulletsAnim); bul != end(explodingBulletsAnim); ) {

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

    if (player->exploding) {
        player->explodingAnimFramesCounter--;

        if (player->explodingAnimFramesCounter < 0) {
            printf("GAME-OVER_GAME-OVER_GAME-OVER_GAME-OVER_GAME-OVER_GAME-OVER_GAME-OVER_\n");

            player->explodingAnimFramesCounter = 20;
            player->exploding = false;
        }
    }
}

void collisionDetector(std::vector<Bullet>& playerBullets, std::vector<Enemy>& enemies, std::array<Bunker*, 3>& bunkers, std::vector<EnemyBullet>& enemiesBullet, Player* player) {

    for (auto& bul = begin(playerBullets); bul != end(playerBullets); bul++) {

        // Check collisions of player's bullets on bunker1's slices
        if (bunkers[0]) {
            if (unsigned flag = bunkers[0]->collisionDetector(bul._Ptr)) {

                bul->exploding = true;
                explodingBulletsAnim.push_back(*bul._Ptr);
                bul->position.x = DEBUG_X;

                // Bunker has been completely destroyed
                if (flag == 2) {
                    delete bunkers[0];
                    bunkers[0] = nullptr;
                    printf("___bunker.0__DESTROYED______\n");
                }
                continue;
            }
        }
        if (bunkers[1]) {
            if (unsigned flag = bunkers[1]->collisionDetector(bul._Ptr)) {

                bul->exploding = true;
                explodingBulletsAnim.push_back(*bul._Ptr);
                bul->position.x = DEBUG_X;

                // Bunker has been completely destroyed
                if (flag == 2) {
                    delete bunkers[1];
                    bunkers[1] = nullptr;
                    printf("___bunker.1__DESTROYED______\n");
                }
                continue;
            }
        }
        if (bunkers[2]) {
            if (unsigned flag = bunkers[2]->collisionDetector(bul._Ptr)) {

                bul->exploding = true;
                explodingBulletsAnim.push_back(*bul._Ptr);
                bul->position.x = DEBUG_X;

                // Bunker has been completely destroyed
                if (flag == 2) {
                    delete bunkers[2];
                    bunkers[2] = nullptr;
                    printf("___bunker.2__DESTROYED______\n");
                }
                continue;
            }
        }

        // Check collisions of player's bullets on enemies grid
        for (auto& enemy = begin(enemies); enemy != end(enemies); ) {
            // For each shotted player's bullet check collision with each enemy...
            if (CheckCollisionRecs(Rectangle{ enemy->position.x, enemy->position.y, enemy->enemy_T1.width * 1.0f, enemy->enemy_T1.height * 1.0f },
                Rectangle{ bul->position.x, bul->position.y, bul->bullet_T.width * 1.0f, bul->bullet_T.height * 1.0f })) {

                // Updates enemies AI structure
                enemiesColumnsDeathCounter[enemy->gridX - 1]++;
                //----------------------------------------------------------------------------------

                printf("___enemy__HIT__<x:%d><y:%d>______\n", enemy->gridX, enemy->gridY);

                PlaySound(fxEnemyExplosion);

                enemy->exploding = true;
                explodingEnemiesAnim.push_back(*enemy._Ptr);
                enemy = enemies.erase(enemy);
                bul->position.x = DEBUG_X;
                break;
            }
            else
                enemy++;
        }

        // Check collisions of player's bullets on enemies's bullets
        for (auto& enBul = begin(enemiesBullet); enBul != end(enemiesBullet);) {

            if (CheckCollisionRecs(Rectangle{ bul->position.x, bul->position.y, bul->bullet_T.width * 1.0f, bul->bullet_T.height * 1.0f },
                Rectangle{ enBul->position.x, enBul->position.y, enBul->bullet_T.width * 1.0f, enBul->bullet_T.height * 1.0f })) {

                printf("___bullets__HIT__<pl.bul.x:%d><pl.bul.y:%d>---<en.bul.x:%d><en.bul.y:%d>______\n", (int)bul->position.x, (int)bul->position.y, (int)enBul->position.x, (int)enBul->position.y);

                // Player bullet start explosion
                bul->exploding = true;
                explodingBulletsAnim.push_back(*bul._Ptr);
                //bul = playerBullets.erase(bul);
                bul->position.x = DEBUG_X;

                // Enemy bullet start explosion
                enBul->exploding = true;
                explodingBulletsAnim.push_back(*enBul._Ptr);
                enBul = enemiesBullet.erase(enBul);
                break;
            }
            else 
                enBul++;
        }
    }

    
    for (auto& enBul = begin(enemiesBullet); enBul != end(enemiesBullet); enBul++) {

        // Check collisions of enemies's bullets on player
        if (player->collisionDetector(enBul._Ptr)) {

            PlaySound(fxPlayerExplosion);

            enBul->exploding = true;
            explodingBulletsAnim.push_back(*enBul._Ptr);
            enBul->position.x = DEBUG_X;
            continue;
        }

        // Check collisions of enemies's bullets on bunker's slices
        if (bunkers[0]) {
            if (unsigned flag = bunkers[0]->collisionDetector(enBul._Ptr)) {

                enBul->exploding = true;
                explodingBulletsAnim.push_back(*enBul._Ptr);
                enBul->position.x = DEBUG_X;

                // Bunker has been completely destroyed
                if (flag == 2) {
                    delete bunkers[0];
                    bunkers[0] = nullptr;
                    printf("___bunker.0__DESTROYED______\n");
                }
                continue;
            }
        }
        if (bunkers[1]) {
            if (unsigned flag = bunkers[1]->collisionDetector(enBul._Ptr)) {

                enBul->exploding = true;
                explodingBulletsAnim.push_back(*enBul._Ptr);
                enBul->position.x = DEBUG_X;

                // Bunker has been completely destroyed
                if (flag == 2) {
                    delete bunkers[1];
                    bunkers[1] = nullptr;
                    printf("___bunker.1__DESTROYED______\n");
                }
                continue;
            }
        }
        if (bunkers[2]) {
            if (unsigned flag = bunkers[2]->collisionDetector(enBul._Ptr)) {

                enBul->exploding = true;
                explodingBulletsAnim.push_back(*enBul._Ptr);
                enBul->position.x = DEBUG_X;

                // Bunker has been completely destroyed
                if (flag == 2) {
                    delete bunkers[2];
                    bunkers[2] = nullptr;
                    printf("___bunker.2__DESTROYED______\n");
                }
                continue;
            }
        }
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
                    if (enemy.gridY == row)
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

int static_AI(float playerX, std::vector<Enemy>& enemies) {

    for (auto enemy = begin(enemies); enemy != end(enemies); enemy++) {

        int x = enemy->gridX - 1;
        int offset = 4 - enemiesColumnsDeathCounter[x];

        enemy += offset;
        float enemyX = enemy->position.x + enemy->enemy_T1.width / 2;

        if (debug_mode == 1)
            debug_text[7] = "out_of_range";

        // Player is inside range of the current enemy => SHOT
        if (playerX <= enemyX + 12.0 && playerX >= enemyX - 12.0) {

            if (debug_mode == 1) {
                enemy->AI_target = true;
                debug_text[7] = std::to_string(static_cast<int>(playerX));
            }

            attacker = enemy._Ptr;
            return 1;
        }

        enemy->AI_target = false;

    }

    attacker = nullptr;
    return 0;
}

int predictive_AI(Player* player, std::vector<Enemy>& enemies, int direction) {

    /**
    * 1. get a starting point, based on playerDirection enemy.x position
    *   => to left = begin
    *   => to right = end
    * 2. calculate player.x in Z frame forward, where Z is the frames needed to a faster bullet of a potential enemy
    */

    float frameTime = GetFrameTime();

    for (auto enemy = begin(enemies); enemy != end(enemies); enemy++) {

        int x = enemy->gridX - 1;
        int offset = 4 - enemiesColumnsDeathCounter[x];
        enemy += offset;
        enemy->AI_target = false;

        float enemyX = enemy->position.x + enemy->enemy_T1.width / 2;

        float framesBullet = ((player->position.y + 3) - (enemy->position.y + allTexture.at(ENEMY_FASTER_BULLET_1_T).height)) / (FASTER * frameTime); // Frames needed to a bullet to reach player.y
        float predictive_x = nextPl_x + (player->speed * frameTime * framesBullet * direction);

        if (debug_mode == 1)
            debug_text[7] = "out_of_range";

        // Player will be inside range of this enemy => PREVENTIVE SHOT
        if (predictive_x <= enemyX + 19.0 && predictive_x >= enemyX - 19.0) {

            if (debug_mode == 1) {
                enemy->AI_target = true;
                debug_text[7] = std::to_string(static_cast<int>(predictive_x));
            }

            attacker =  enemy._Ptr;
            return 2;
        }
    }

    attacker = nullptr;
    return 0;
}

int bunkerAttack_AI(Bunker* bunker, std::vector<Enemy>& enemies) {

    if (debug_mode == 1) {
        debug_text[3] = "out_of_range";
        debug_text[5] = "bunkerAttack_AI";
    }

    float sliceX = 0.0f;

    if (bunker->topLeft || bunker->centerLeft || bunker->bottomLeft)
        sliceX = bunker->spawn.x + 12.0f;
    else if(bunker->topCenter || bunker->centerCenter)
        sliceX = bunker->spawn.x + 36.0f;
    else
        sliceX = bunker->spawn.x + 60.0f;

    for (auto enemy = begin(enemies); enemy != end(enemies); enemy++) {

        int x = enemy->gridX - 1;
        int offset = 4 - enemiesColumnsDeathCounter[x];

        enemy += offset;
        float enemyX = enemy->position.x + enemy->enemy_T1.width / 2;

        // Bunker is inside range of the current enemy => SHOT
        if (sliceX <= enemyX + 12.0 && sliceX >= enemyX - 12.0) {

            if (debug_mode == 1) {
                enemy->AI_target = true;
                debug_text[7] = std::to_string(static_cast<int>(sliceX));
            }

            attacker = enemy._Ptr;
            return 1;
        }

        enemy->AI_target = false;

    }

    attacker = nullptr;
    return 0;
}

// Real-time scheduling of an enemy that should shot to the player 
int AI(Player* player, std::vector<Enemy>& enemies,unsigned state) {

    /**
    * 1. get player's first position.
    * 2. wait 5 frame
    * 3. get player's second position.
    * 4. evaluate if player is still or moving
    *   | => still = static_AI()
    *   | => moving = predictive_AI()
    */

    if (state == 0) {
        currPl_x = player->position.x + player->player_T.width / 2;
        return 0;
    }

    nextPl_x = player->position.x + player->player_T.width / 2;

    if (nextPl_x < currPl_x) {                          // Player is moving to left
        return predictive_AI(player, enemies, -1);
    }
    else if (nextPl_x > currPl_x) {                       // Player is moving to right
        return predictive_AI(player, enemies, +1);
    }
    else {
        return static_AI(nextPl_x, enemies);            // Player is still
    }
}

// Handle all updates before drawing
void updateManager(Player* player, std::vector<Bullet>& playerBullets, std::vector<EnemyBullet>& enemiesBullets, std::vector<Enemy>& enemies, std::array<Bunker*, 3>& bunkers) {

    keyboardEventsHandler(player, playerBullets, enemiesBullets);
    explodingAnimHandler(player);
    bulletsMovementsHandler(playerBullets, enemiesBullets);
    collisionDetector(playerBullets, enemies, bunkers, enemiesBullets, player);
}

//----------------------------------------------------------------------------------
//      DRAWS
//----------------------------------------------------------------------------------

// Manage all text to draw
void textHandler() {

    int aux = 0;
    int newLine = 15;

    //DrawTextEx(font, "GAMEPLAY SCREEN", Vector2{ 20, 10 }, font.baseSize * 1.0f, 4, Color{ 190, 33, 55, 100 });
    //DrawText("Movement: \tW - A\nShot: \tSPACE", 25, 30, 18, MAROON);
    DrawFPS(GetScreenWidth() - 100, 10);

    // ToastMsg cases based on keyboard events
    switch (textState) {

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

    // __debug_mode
    for (int i = 0; i < 128; i++) {
        if (debug_text[i].size() == 0)
            break;

        if (i == 0) {
            aux = MeasureText(debug_text[i].c_str(), 14);
            DrawText(debug_text[i].c_str(), 10, 5, 14, MAROON);
            continue;
        }
        else if (i == 1) {
            aux = MeasureText(debug_text[i].c_str(), 14);
            DrawText(debug_text[i].c_str(), 120, 5, 14, PURPLE);
            continue;
        }
        else if (i % 2 == 0) {
            aux = MeasureText(debug_text[i].c_str(), 13);
            DrawText(debug_text[i].c_str(), 10, 5 + newLine, 13, RED);
            aux = MeasureText(debug_text[i].c_str(), 13);
            DrawText(debug_text[++i].c_str(), 120, 5 + newLine, 13, YELLOW);
        }

        newLine += 15;
    }
}

// Handle all elements to draw
void drawManager(Player* player, std::vector<Bullet>& playerBullets, std::vector<Enemy>& enemies, std::vector<EnemyBullet>& enemiesBullets, std::array<Bunker*, 3>& bunkers) {

    player->draw();
    if(bunkers[0]) bunkers[0]->draw();
    if(bunkers[1]) bunkers[1]->draw();
    if(bunkers[2]) bunkers[2]->draw();
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

    // Draw exploding enemies animation
    for (auto& explodingEnemy : explodingEnemiesAnim) {
        explodingEnemy.draw();
    }
}

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(std::vector<Enemy>& enemies)
{
    //__debug_mode
    debug_mode = 0;

    // View
    finishScreen = 0;
    textState = 0;
    textFramesCounter = 0;
    framesCounter = 0;
    musicState = true;
    std::sprintf(toastMsg, " ");
    //----------------------------------------------------------------------------------
    // Gameplay
    SPEED = MOVEMENT_SPEED;
    ROWS_SPEED = ROW_GRADUAL_SPEED;
    enemyShouldMove = false;
    //----------------------------------------------------------------------------------
    // Animation
    currRowEnemies = 1;

    if (enemies.front().direction == -1)
        enemyCurrentDirection = Movement::LEFT;
    else
        enemyCurrentDirection = Movement::RIGHT;
    //----------------------------------------------------------------------------------
    // Enemies
    enemiesColumnsDeathCounter.fill(0);
    attacker = nullptr;
    enemyShotWait = ENEMIES_WAIT_BEFORE_SHOT;
    enemyHasShot = false;
    enemyCanShot = true;
    // predictive_AI
    AI_framesCounter = AI_REFRESH_RATE;
    currPl_x = 0.0f;
    nextPl_x = 0.0f;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(Player* player, std::vector<Bullet>& playerBullets, std::vector<EnemyBullet>& enemiesBullets, std::vector<Enemy>& enemies, std::array<Bunker*, 3>& bunkers)
{
    // This will allow enemies to shot their bullet each ENEMIES_WAIT_BEFORE_SHOT frames and not all instantly
    if (enemyHasShot) {
        enemyShotWait--;

        if (enemyShotWait <= 0) {
            enemyCanShot = true;
            enemyHasShot = false;
            enemyShotWait = ENEMIES_WAIT_BEFORE_SHOT;

            if(debug_mode == 2)
                debug_text[7] = std::to_string(enemyShotWait);
        }
    }

    updateManager(player, playerBullets, enemiesBullets, enemies, bunkers);

    if (AI_framesCounter == AI_REFRESH_RATE)
        AI(player, enemies, 0);
    else if (AI_framesCounter == 0) {

        int result_AI = AI(player, enemies, 1);

        // Static_AI
        if (result_AI == 1 && enemiesBullets.size() < ENEMIES_MAX_BULLET && enemyCanShot) {

            if (debug_mode == 1) {
                debug_text[3] = "still";
                debug_text[5] = "static_AI";
            }

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

            enemyCanShot = false;
            enemyHasShot = true;
        }
        // Predictive_AI
        else if (result_AI == 2 && enemiesBullets.size() < ENEMIES_MAX_BULLET && enemyCanShot) {

            if (debug_mode == 1) {
                debug_text[3] = "moving";
                debug_text[5] = "predictive_AI";
            }

            enemiesBullets.push_back(EnemyBullet(allTexture.at(ENEMY_FASTER_BULLET_1_T), allTexture.at(ENEMY_FASTER_BULLET_2_T), allTexture.at(ENEMY_FASTER_BULLET_3_T),
                allTexture.at(ENEMY_FASTER_BULLET_4_T), allTexture.at(TextureIndexes::ENEMY_BULLET_EXPLODING_T),
                Position{ attacker->position.x + (attacker->enemy_T1.width / 2), attacker->position.y }, BulletType::FASTER_BULLET));

            enemyCanShot = false;
            enemyHasShot = true;
        }
        // Bunkers_AI
        else if (result_AI == 0 && enemiesBullets.size() < ENEMIES_MAX_BULLET && enemyCanShot) {

            if (bunkers[0] && bunkerAttack_AI(bunkers[0], enemies)) {
                enemiesBullets.push_back(EnemyBullet(allTexture.at(ENEMY_POWERFUL_BULLET_1_T), allTexture.at(ENEMY_POWERFUL_BULLET_2_T), allTexture.at(ENEMY_POWERFUL_BULLET_3_T),
                    allTexture.at(ENEMY_POWERFUL_BULLET_4_T), allTexture.at(TextureIndexes::ENEMY_BULLET_EXPLODING_T),
                    Position{ attacker->position.x + (attacker->enemy_T1.width / 2), attacker->position.y }, BulletType::POWERFUL_BULLET));

                enemyCanShot = false;
                enemyHasShot = true;
            }
            else if (bunkers[1] && bunkerAttack_AI(bunkers[1], enemies)) {
                enemiesBullets.push_back(EnemyBullet(allTexture.at(ENEMY_POWERFUL_BULLET_1_T), allTexture.at(ENEMY_POWERFUL_BULLET_2_T), allTexture.at(ENEMY_POWERFUL_BULLET_3_T),
                    allTexture.at(ENEMY_POWERFUL_BULLET_4_T), allTexture.at(TextureIndexes::ENEMY_BULLET_EXPLODING_T),
                    Position{ attacker->position.x + (attacker->enemy_T1.width / 2), attacker->position.y }, BulletType::POWERFUL_BULLET));

                enemyCanShot = false;
                enemyHasShot = true;
            }
            else if (bunkers[2] && bunkerAttack_AI(bunkers[2], enemies)) {
                enemiesBullets.push_back(EnemyBullet(allTexture.at(ENEMY_POWERFUL_BULLET_1_T), allTexture.at(ENEMY_POWERFUL_BULLET_2_T), allTexture.at(ENEMY_POWERFUL_BULLET_3_T),
                    allTexture.at(ENEMY_POWERFUL_BULLET_4_T), allTexture.at(TextureIndexes::ENEMY_BULLET_EXPLODING_T),
                    Position{ attacker->position.x + (attacker->enemy_T1.width / 2), attacker->position.y }, BulletType::POWERFUL_BULLET));

                enemyCanShot = false;
                enemyHasShot = true;
            }
            else {
                if (debug_mode == 1) {
                    debug_text[3] = "out_of_range";
                    debug_text[5] = "none";
                    debug_text[7] = "no_target";
                }

                enemyCanShot = true;
                enemyHasShot = false;
            }
        }

        AI_framesCounter = AI_REFRESH_RATE + 1;
    }

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
    AI_framesCounter--;
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(Player* player, std::vector<Bullet>& playerBullets, std::vector<EnemyBullet>& enemiesBullets, std::vector<Enemy>& enemies, std::array<Bunker*, 3>& bunkers)
{

    ClearBackground(RAYWHITE);
    DrawTexture(allTexture.at(TextureIndexes::BACKGROUND_T), 0, 0, WHITE);  // Draw background

    drawManager(player, playerBullets, enemies, enemiesBullets, bunkers);            // Draw gameplay's elements

    
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