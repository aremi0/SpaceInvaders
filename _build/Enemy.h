#pragma once
#include "raylib.h"
#include "../_build/typedef.h"

class Enemy {
public:
	int resistence;
	int speed;
	bool exploding;

	float x;							// ...*
	float y;							// *...Absolute position on the screen.
	Position grid;						// Postation on the enemy-grid (first, second, third, ...)
	EnemyType type;

// Textures
	Texture2D enemyExploding_T;

	// Moving Animation Texture
	Texture2D enemy_T1;
	Texture2D enemy_T2;

// Methods
	Enemy(Texture2D enemy_t1, Texture2D enemy_t2, Texture2D enemyExploding_t);
};

