#include "Bunker.h"

Bunker::Bunker(Position sp,
				Texture2D tL1, Texture2D tL2, Texture2D tL3, Texture2D tL4,
				Texture2D c1, Texture2D c2, Texture2D c3, Texture2D c4,
				Texture2D tR1, Texture2D tR2, Texture2D tR3, Texture2D tR4,
				Texture2D bL1, Texture2D bL2, Texture2D bL3, Texture2D bL4,
				Texture2D bR1, Texture2D bR2, Texture2D bR3, Texture2D bR4) {

	this->spawn = sp;

	this->topLeft = BunkerSlice(tL1, tL2, tL2, tL4);
	this->topCenter = BunkerSlice(c1, c2, c3, c4);
	this->topRight = BunkerSlice(tR1, tR2, tR3, tR4);

	this->centerLeft = BunkerSlice(c1, c2, c3, c4);
	this->centerCenter = BunkerSlice(c1, c2, c3, c4);
	this->centerRight = BunkerSlice(c1, c2, c3, c4);

	this->bottomLeft = BunkerSlice(bL1, bL2, bL3, bL4);
	this->bottomRight = BunkerSlice(bR1, bR2, bR3, bR4);
}

void Bunker::draw() {
	
	DrawTexture(topLeft.draw(), this->spawn.x, this->spawn.y, Color{0, 0, 0, 0});
	DrawTexture(topCenter.draw(), this->spawn.x + topLeft.state1.width, this->spawn.y, Color{0, 0, 0, 0});
	DrawTexture(topRight.draw(), this->spawn.x + topLeft.state1.width + topRight.state1.width, this->spawn.y, Color{ 0, 0, 0, 0 });

}