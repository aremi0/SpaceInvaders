#pragma once
#include "raylib.h"
#include "../_build/typedef.h"

class Bullet {
public:
	int speed;
	bool exploding;
	int explodingAnimFramesCounter;
	Position position;
	BulletType type;
// Textures
	Texture2D bullet_T;
	Texture2D bulletExplodind_T;

// Methods
	Bullet(Texture2D bullet_t, Texture2D bulletExploding_t, Position spawn, BulletType type, bool exploding = false);
	void move(Movement direction, float frameTime);
	void draw();
};

