#include "BunkerSlice.h"

BunkerSlice::BunkerSlice(int health, Texture2D t1, Texture2D t2, Texture2D t3, Texture2D t4) {
	this->initialHealth = health;
	this->currentHealth = health;

	this->state1 = t1;
	this->state2 = t2;
	this->state3 = t3;
	this->state4 = t4;
}

Texture2D BunkerSlice::draw(void) {
	if (this->currentHealth >= this->initialHealth * 0.8)
		return this->state1;
	else if (this->currentHealth >= this->initialHealth * 0.5)
		return this->state2;
	else if (this->currentHealth >= this->initialHealth * 0.25)
		return this->state3;
	else
		return this->state4;
}