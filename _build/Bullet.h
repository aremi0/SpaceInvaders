#pragma once
#include "raylib.h"
#include "raymath.h"
#include "../_build/typedef.h"

class Bullet {
public:
// Data
//----------------------------------------------------------------------------------
	// Gameplay data
	int speed;
	bool exploding;
	Position position;
	BulletType type;
	int power;

	// Animation data
	int explodingAnimFramesCounter;

// Textures
//----------------------------------------------------------------------------------
	Texture2D bullet_T;
	Texture2D bulletExplodind_T;

// Methods
//----------------------------------------------------------------------------------
	Bullet(Texture2D bullet_t, Texture2D bulletExploding_t, Position spawn, BulletType type, bool exploding = false);
	void move(float frameTime);
	void draw(void);
};

