#include "Enemy.h"

Enemy::Enemy(Texture2D enemy_t1, Texture2D enemy_t2, Texture2D enemyExploding_t, int x, int y, EnemyType type) {
// Texture init
	this->enemy_T1 = enemy_t1;
	this->enemy_T2 = enemy_t2;
	this->enemyExploding_T = enemyExploding_t;

// Animation init
	this->explodingAnimFramesCounter = 5;

// Data init
	//this->resistence = 1;
	//this->speed = 50;
	this->type = type;
	this->exploding = false;

	this->margin = this->enemy_T1.width + (GetScreenWidth() * 0.02);					 // Margin equal texture width + 2% of the screen width
	this->gridX = x;
	this->gridY = y;
	this->position.x = margin * this->gridX;
	this->position.y = margin * this->gridY;
}

void Enemy::move(Movement direction, float frameTime) {
	if (direction == Movement::DOWN)
		this->position.y += 10;
	else {
		this->position.x += direction * frameTime * 10;
	}
}

void Enemy::draw(bool movementState) {
	// "movementState" determine which movement animation texture draw
	if (exploding) {
		DrawTexture(this->enemyExploding_T, this->position.x, this->position.y, WHITE);
	}
	else {
		DrawTexture(movementState ? this->enemy_T1 : this->enemy_T2, this->position.x, this->position.y, WHITE);
	}

	
}