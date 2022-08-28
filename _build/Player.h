#pragma once
#include "raylib.h"
#include "../_build/typedef.h"

class Player {
public:
// Data
//----------------------------------------------------------------------------------
	int life;
	int speed;
	bool exploding;
	Position position;

//Textures
//----------------------------------------------------------------------------------
	Texture2D player_T;
	Texture2D playerExploding_T;

// Methods
//----------------------------------------------------------------------------------
	Player(Texture2D player_t);
	void move(Movement direction, float frameTime);
	void draw(void);
	void reset(void);
	void hardReset(void);
};

