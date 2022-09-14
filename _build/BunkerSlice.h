#pragma once
#include "raylib.h"
#include "../_build/typedef.h"

class BunkerSlice {
public:
// Data
//----------------------------------------------------------------------------------
	int initialHealth;
	int currentHealth;

// Textures
//----------------------------------------------------------------------------------
	Texture2D state1;
	Texture2D state2;
	Texture2D state3;
	Texture2D state4;

// Methods
//----------------------------------------------------------------------------------
	BunkerSlice(int health, Texture2D t1, Texture2D t2, Texture2D t3, Texture2D t4);
	Texture2D draw(void);
};