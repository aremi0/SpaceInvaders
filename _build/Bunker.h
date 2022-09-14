#pragma once
#include "raylib.h"
#include "../_build/typedef.h"
#include "BunkerSlice.h"

class Bunker {
public:
// Data
//----------------------------------------------------------------------------------
	BunkerSlice topLeft;
	BunkerSlice topCenter;
	BunkerSlice topRight;

	BunkerSlice centerLeft;
	BunkerSlice centerCenter;
	BunkerSlice centerRight;

	BunkerSlice downLeft;
	BunkerSlice downRight;

// Methods
//----------------------------------------------------------------------------------
	Bunker(Texture2D tL1, Texture2D tL2, Texture2D tL3, Texture2D tL4,
			Texture2D c1, Texture2D c2, Texture2D c3, Texture2D c4,
			Texture2D tR1, Texture2D tR2, Texture2D tR3, Texture2D tR4,
			Texture2D dL1, Texture2D dL2, Texture2D dL3, Texture2D dL4,
			Texture2D dR1, Texture2D dR2, Texture2D dR3, Texture2D dR4);
};