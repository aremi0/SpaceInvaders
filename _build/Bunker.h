#pragma once
#include "raylib.h"
#include "../_build/typedef.h"
#include "BunkerSlice.h"

class Bunker {
public:
// Data
//----------------------------------------------------------------------------------
	Position spawn;

	// Bunker slices
	BunkerSlice topLeft;
	BunkerSlice topCenter;
	BunkerSlice topRight;

	BunkerSlice centerLeft;
	BunkerSlice centerCenter;
	BunkerSlice centerRight;

	BunkerSlice bottomLeft;
	BunkerSlice bottomRight;

// Methods
//----------------------------------------------------------------------------------
	Bunker(	Position sp,
			Texture2D tL1, Texture2D tL2, Texture2D tL3, Texture2D tL4,
			Texture2D c1, Texture2D c2, Texture2D c3, Texture2D c4,
			Texture2D tR1, Texture2D tR2, Texture2D tR3, Texture2D tR4,
			Texture2D bL1, Texture2D bL2, Texture2D bL3, Texture2D bL4,
			Texture2D bR1, Texture2D bR2, Texture2D bR3, Texture2D bR4);

	void draw();
};