/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Screens Functions Declarations (Init, Update, Draw, Unload)
* 
*
**********************************************************************************************/

#pragma once
#include <vector>
#include <array>
#include "../_build/typedef.h"
#include "../_build/Player.h"
#include "../_build/Bullet.h"
#include "../_build/Enemy.h"
#include "../_build/EnemyBullet.h"
#include "../_build/Bunker.h"

#ifndef SCREENS_H
#define SCREENS_H

//----------------------------------------------------------------------------------
// Global Variables Declaration (shared by several modules)
//----------------------------------------------------------------------------------
extern GameScreen currentScreen;
extern Font font;
extern Music music;
extern Sound fxCoin;
extern Sound fxBulletShot;
extern Sound fxPlayerExplosion;
extern Sound fxEnemyExplosion;
extern Sound fxEnemyMove;

extern std::vector<Texture2D> allTexture;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Logo Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitLogoScreen(void);
void UpdateLogoScreen(void);
void DrawLogoScreen(void);
void UnloadLogoScreen(void);
int FinishLogoScreen(void);

//----------------------------------------------------------------------------------
// Title Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
void UnloadTitleScreen(void);
int FinishTitleScreen(void);

//----------------------------------------------------------------------------------
// Options Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitOptionsScreen(void);
void UpdateOptionsScreen(void);
void DrawOptionsScreen(void);
void UnloadOptionsScreen(void);
int FinishOptionsScreen(void);

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitGameplayScreen(std::vector<Enemy>& enemies);
void UpdateGameplayScreen(Player* player, std::vector<Bullet>& playerBullets, std::vector<EnemyBullet>& enemiesBullets, std::vector<Enemy>& enemies, Bunker* bunker1);
void DrawGameplayScreen(Player* player, std::vector<Bullet>& playerBullets, std::vector<EnemyBullet>& enemiesBullets, std::vector<Enemy>& enemies, Bunker* bunker1);
void UnloadGameplayScreen(void);
int FinishGameplayScreen(void);

//----------------------------------------------------------------------------------
// Ending Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitEndingScreen(void);
void UpdateEndingScreen(void);
void DrawEndingScreen(void);
void UnloadEndingScreen(void);
int FinishEndingScreen(void);

#ifdef __cplusplus
}
#endif

#endif // SCREENS_H