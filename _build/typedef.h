#pragma once

//----------------------------------------------------------------------------------
// Strutture e macro di supporto, utili in più screen_view e classi
//----------------------------------------------------------------------------------

#define MAX_PLAYER_BULLETS 2
#define PLAYER_SPAWNPOINT 90
#define MAX_ENEMYS_ROW 5
#define MAX_ENEMYS_COLUMN 11

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

	enum BulletType { PLAYER_BULLET, ENEMY_BULLET };

#endif // !DEF_BULLET

#ifndef DEF_TEXTURE_INDEX

	enum TextureIndexes {
		BACKGROUND_T = 0, PLAYER_T, PLAYER_EXPLODING_1_T, PLAYER_EXPLODING_2_T, PLAYER_BULLET_T, PLAYER_BULLET_EXPLODING_T,
		ENEMY_EXPLODING_T, ENEMY_UFO_T, ENEMY_UFO_EXPLODING_T, ENEMY_SQUID_1_T, ENEMY_SQUID_2_T, ENEMY_CRAB_1_T, ENEMY_CRAB_2_T,
		ENEMY_OCTOPUS_1_T, ENEMY_OCTOPUS_2_T, ENEMY_BULLET_1_T, ENEMY_BULLET_2_T, ENEMY_BULLET_3_T, ENEMY_BULLET_4_T,
		ENEMY_BULLET_EXPLODING_T,
	};

#endif // !DEF_TEXTURE_INDEX

#ifndef DEF_ENEMY_TYPE

	enum EnemyType { UFO = 0, SQUID, CRAB, OCTOPUS };

#endif // !DEF_ENEMY_TYPE