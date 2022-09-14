#include "Bunker.h"

Bunker::Bunker(Texture2D tL1, Texture2D tL2, Texture2D tL3, Texture2D tL4,
				Texture2D c1, Texture2D c2, Texture2D c3, Texture2D c4,
				Texture2D tR1, Texture2D tR2, Texture2D tR3, Texture2D tR4,
				Texture2D dL1, Texture2D dL2, Texture2D dL3, Texture2D dL4,
				Texture2D dR1, Texture2D dR2, Texture2D dR3, Texture2D dR4) {

	this->topLeft = BunkerSlice(tL1, tL2, tL2, tL4);
	this->topCenter = BunkerSlice(c1, c2, c3, c4);
	this->topRight = BunkerSlice(tR1, tR2, tR3, tR4);

	this->centerLeft = BunkerSlice(c1, c2, c3, c4);
	this->centerCenter = BunkerSlice(c1, c2, c3, c4);
	this->centerRight = BunkerSlice(c1, c2, c3, c4);

	this->downLeft = BunkerSlice(dL1, dL2, dL3, dL4);
	this->downRight = BunkerSlice(dR1, dR2, dR3, dR4);
}