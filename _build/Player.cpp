#include "Player.h"

Player::Player(Texture2D player_t, Texture2D playerExploding_t_1, Texture2D playerExploding_t_2) {
// Texture init
	this->player_T = player_t;
	this->playerExploding_T_1 = playerExploding_t_1;
	this->playerExploding_T_2 = playerExploding_t_2;

// Animation init
	this->exploding = false;
	this->explodingAnimFramesCounter = 60;

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
	this->reset();
}

// Screen_gameplay will check limits of the screen
void Player::move(Movement direction, float frameTime) {
	this->position.x += ((direction == Movement::RIGHT ? +1 : -1) * this->speed * frameTime);
}

void Player::draw(void) {

	if(!exploding)
		DrawTexture(this->player_T, this->position.x, this->position.y, GREEN);
	else {
		if(this->explodingAnimFramesCounter > 30)
			DrawTexture(this->playerExploding_T_1, this->position.x, this->position.y, WHITE);
		else
			DrawTexture(this->playerExploding_T_2, this->position.x, this->position.y, WHITE);
	}
}

bool Player::collisionDetector(Bullet* bullet) {

	if (CheckCollisionRecs(Rectangle{ this->position.x, this->position.y, this->player_T.width * 1.0f, this->player_T.height * 1.0f },
		Rectangle{ bullet->position.x, bullet->position.y, bullet->bullet_T.width * 1.0f, bullet->bullet_T.height * 1.0f })) {

		printf("___player__HIT__<b.type:%d>___GAME-OVER___\n", (int)bullet->type);

		this->exploding = true;

		return true;
	}

	return false;
}

