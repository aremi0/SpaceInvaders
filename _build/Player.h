#pragma once
#include "raylib.h"
#include "typedef.h"
#include "bullet.h"
#include <stdio.h>

class Player {
public:
// Data
	int speed;
	Position position;
	bool exploding;
	int explodingAnimFramesCounter;

//Textures
	Texture2D player_T;
	Texture2D playerExploding_T_1;
	Texture2D playerExploding_T_2;

// Methods
	Player(Texture2D player_t, Texture2D playerExploding_t_1, Texture2D playerExploding_t_2);
	void move(Movement direction, float frameTime);
	void draw(void);
	void reset(void);
	void hardReset(void);
	bool collisionDetector(Bullet* bullet);
};

