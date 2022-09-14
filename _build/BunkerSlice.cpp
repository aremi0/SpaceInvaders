#include "BunkerSlice.h"

BunkerSlice::BunkerSlice() { }

BunkerSlice::BunkerSlice(Texture2D t1, Texture2D t2, Texture2D t3, Texture2D t4) {
	this->health = 4000;
	this->state1 = t1;
	this->state2 = t2;
	this->state3 = t3;
	this->state4 = t4;
}

Texture2D BunkerSlice::draw() {
	if (health > 3500)
		return this->state1;
	else if (health > 2500)
		return this->state2;
	else if (health > 1500)
		return this->state3;

}