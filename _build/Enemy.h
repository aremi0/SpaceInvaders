#pragma once
#include "raylib.h"
#include "../_build/typedef.h"

class Enemy {
public:
// Data
//----------------------------------------------------------------------------------
	// Gameplay data
	//int resistence;
	//int speed;
	bool exploding;
	EnemyType type;

	float margin;
	int gridX;								// ...*
	int gridY;								// *... Postation on the enemy-grid (first, second, third, ...)
	Position position;						// Absolute position on the screen.

	//Animation data
	int explodingAnimFramesCounter;

// Textures
//----------------------------------------------------------------------------------
	Texture2D enemyExploding_T;
	Texture2D enemy_T1;
	Texture2D enemy_T2;

// Methods
//----------------------------------------------------------------------------------
	Enemy(Texture2D enemy_t1, Texture2D enemy_t2, Texture2D enemyExploding_t, int x, int y, EnemyType type);
	void move(Movement direction, float frameTime);
	void draw(bool movementState);
};

