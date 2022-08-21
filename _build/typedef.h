#pragma once

//----------------------------------------------------------------------------------
// Strutture e macro di supporto, utili in più screen_view e classi
//----------------------------------------------------------------------------------

#define MAX_PLAYER_BULLETS 2
#define PLAYER_SPAWNPOINT 90

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

	enum Movement { UP = -1, DOWN = +1, LEFT = -1, RIGHT = +1 };

#endif // !DEF_MOVEMENT

#ifndef DEF_BULLET

	enum BulletType { PLAYER_BULLET, ENEMY_BULLET };

#endif // !DEF_BULLET

#ifndef DEF_TEXTURE_INDEX

	enum TextureIndexes { BACKGROUND_T = 0, PLAYER_T, PLAYER_BULLET_T, PLAYER_BULLET_EXPLODING_T };

#endif // !DEF_TEXTURE_INDEX

#ifndef DEF_ENEMY_TYPE

	enum EnemyType { CRAB, OCTOPUS, SQUID, UFO };

#endif // !DEF_ENEMY_TYPE