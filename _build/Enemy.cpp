#include "Enemy.h"

Enemy::Enemy(Texture2D enemy_t1, Texture2D enemy_t2, Texture2D enemyExploding_t, int x, int y, EnemyType type) {
// Texture init
	this->enemy_T1 = enemy_t1;
	this->enemy_T2 = enemy_t2;
	this->enemyExploding_T = enemyExploding_t;

// Animation init
	this->movementState = true;															// Determine which movement animation texture draw
	this->explodingAnimFramesCounter = 7;

// Data init
	//this->resistence = 1;
	this->direction = +1;																// RIGHT
	this->type = type;
	this->exploding = false;
	this->AI_target = false;

	this->gridX = x;
	this->gridY = y;
	this->position.x = this->enemy_T1.width + (GetScreenWidth() * 0.075) * this->gridX;
	this->position.y = this->enemy_T1.height + (GetScreenWidth() * 0.06) * this->gridY;	//... Setting a margin for each enemy
}

void Enemy::move(float frameTime, bool goDown) {
	if (goDown) {
		this->position.y += GetScreenHeight() * 0.025;
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

	// If selected by AI to shot the player, draw a rectangle line to evidence it...
	if (AI_target) {
		DrawRectangleLines(this->position.x, this->position.y, this->enemy_T1.width, this->enemy_T1.height, RED);
		DrawLine((this->position.x + this->enemy_T1.width / 2), this->position.y, (this->position.x + this->enemy_T1.width / 2), GetScreenHeight(), YELLOW);
	}
}
