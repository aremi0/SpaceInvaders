#include "Player.h"

Player::Player(Texture2D player_t) {
// Texture init
	this->player_T = player_t;

// Animation init
	this->exploding = false;

// Data init
	this->hardReset();
	this->position.y = GetScreenHeight() - SCREEN_HEIGHT_MARGIN;
	this->speed = 150;
}

// Reset just the variables to start a new wave (next-level)
void Player::reset(void) {
	this->position.x = PLAYER_SPAWNPOINT;
}

// Reset alla variables to start a new game
void Player::hardReset(void) {
	this->life = 3;
	this->reset();
}

// Screen_gameplay will check limits of the screen
void Player::move(Movement direction, float frameTime) {
	this->position.x += ((direction == Movement::RIGHT ? +1 : -1) * this->speed * frameTime);
}

void Player::draw(void) {
	DrawTexture(this->exploding ? this->playerExploding_T : this->player_T, this->position.x, this->position.y, GREEN);
}