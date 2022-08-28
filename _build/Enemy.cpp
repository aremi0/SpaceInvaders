#include "Enemy.h"

Enemy::Enemy(Texture2D enemy_t1, Texture2D enemy_t2, Texture2D enemyExploding_t, int x, int y, EnemyType type) {
// Texture init
	this->enemy_T1 = enemy_t1;
	this->enemy_T2 = enemy_t2;
	this->enemyExploding_T = enemyExploding_t;

// Animation init
	this->movementState = true;															// Determine which movement animation texture draw
	this->explodingAnimFramesCounter = 5;

// Data init
	//this->resistence = 1;
	this->direction = +1;
	this->type = type;
	this->exploding = false;

	this->margin = this->enemy_T1.width + (GetScreenWidth() * 0.01);					 // Margin equal texture width + 1% of the screen width
	this->gridX = x;
	this->gridY = y;
	this->position.x = margin * this->gridX;
	this->position.y = SCREEN_HEIGHT_MARGIN * this->gridY;
}

void Enemy::move(float frameTime, bool goDown) {
	if (goDown) {
		this->position.y += 10;
		this->direction *= -1;
	}
	else {
		this->position.x += this->direction * frameTime * (GetScreenWidth() * 0.9);
	}

	this->movementState = !this->movementState;
}

void Enemy::draw(void) {
	// "movementState" determine which movement animation texture draw
	if (exploding) {
		DrawTexture(this->enemyExploding_T, this->position.x, this->position.y, WHITE);
	}
	else {
		DrawTexture(this->movementState ? this->enemy_T1 : this->enemy_T2, this->position.x, this->position.y, WHITE);
	}

	
}

Enemy* Enemy::StartExploding(void) {
	this->exploding = true;
	return this;
}