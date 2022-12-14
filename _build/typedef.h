#pragma once

//----------------------------------------------------------------------------------
// Strutture e macro di supporto, utili in pi? screen_view e classi
//----------------------------------------------------------------------------------

#define FASTER 280
#define MAX_PLAYER_BULLETS 2
#define PLAYER_SPAWNPOINT 90
#define MAX_ENEMIES_COLUMN 11
#define DEBUG_X -100

extern float SCREEN_WIDTH_MARGIN;
extern float SCREEN_HEIGHT_MARGIN;

#ifndef DEF_POSITION

	struct Position {
		float x;
		float y;
	};

#endif // !DEF_POSITION

#ifndef DEF_GAMESCREEN

	typedef enum GameScreen { _DEB = -1, LOGO = 0, TITLE, OPTIONS, GAMEPLAY, ENDING } GameScreen;

#endif // !DEF_GAMESCREEN

#ifndef DEF_MOVEMENT

	enum Movement { UP = 0, DOWN, LEFT, RIGHT };

#endif // !DEF_MOVEMENT

#ifndef DEF_BULLET

	enum BulletType { PLAYER_BULLET, SLOWER_BULLET, FASTER_BULLET, POWERFUL_BULLET };

#endif // !DEF_BULLET

#ifndef DEF_TEXTURE_INDEX

	enum TextureIndexes {
		BACKGROUND_T = 0, PLAYER_T, PLAYER_EXPLODING_1_T, PLAYER_EXPLODING_2_T, PLAYER_BULLET_T, PLAYER_BULLET_EXPLODING_T,
		ENEMY_EXPLODING_T, ENEMY_UFO_T, ENEMY_UFO_EXPLODING_T, ENEMY_SQUID_1_T, ENEMY_SQUID_2_T, ENEMY_CRAB_1_T, ENEMY_CRAB_2_T,
		ENEMY_OCTOPUS_1_T, ENEMY_OCTOPUS_2_T, ENEMY_BULLET_EXPLODING_T, ENEMY_SLOW_BULLET_1_T, ENEMY_SLOW_BULLET_2_T, ENEMY_SLOW_BULLET_3_T, ENEMY_SLOW_BULLET_4_T,
		ENEMY_FASTER_BULLET_1_T, ENEMY_FASTER_BULLET_2_T, ENEMY_FASTER_BULLET_3_T, ENEMY_FASTER_BULLET_4_T, ENEMY_POWERFUL_BULLET_1_T, ENEMY_POWERFUL_BULLET_2_T,
		ENEMY_POWERFUL_BULLET_3_T, ENEMY_POWERFUL_BULLET_4_T, BUNKER_TOP_LEFT_1_T, BUNKER_TOP_LEFT_2_T, BUNKER_TOP_LEFT_3_T, BUNKER_TOP_LEFT_4_T,
		BUNKER_CENTER_1_T, BUNKER_CENTER_2_T, BUNKER_CENTER_3_T, BUNKER_CENTER_4_T, BUNKER_TOP_RIGHT_1_T, BUNKER_TOP_RIGHT_2_T, BUNKER_TOP_RIGHT_3_T, BUNKER_TOP_RIGHT_4_T,
		BUNKER_BOTTOM_LEFT_1_T, BUNKER_BOTTOM_LEFT_2_T, BUNKER_BOTTOM_LEFT_3_T, BUNKER_BOTTOM_LEFT_4_T,
		BUNKER_BOTTOM_RIGHT_1_T, BUNKER_BOTTOM_RIGHT_2_T, BUNKER_BOTTOM_RIGHT_3_T, BUNKER_BOTTOM_RIGHT_4_T,
	};

#endif // !DEF_TEXTURE_INDEX

#ifndef DEF_ENEMY_TYPE

	enum EnemyType { UFO = 0, SQUID, CRAB, OCTOPUS };

#endif // !DEF_ENEMY_TYPE