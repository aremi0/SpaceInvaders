#include "Bullet.h"
#include "raymath.h"

Bullet::Bullet(Texture2D bullet_t, Texture2D bulletExploding_t, Position spawn, BulletType type, bool exploding) {
// Texture init
	this->bullet_T = bullet_t;
	this->bulletExplodind_T = bulletExploding_t;
	
// Animation init
	this->exploding = exploding;
	this->explodingAnimFramesCounter = 5;

// Data init
	this->position.x = spawn.x;
	this->position.y = spawn.y;
	this->type = type;

	//BulletType dependance data
	if (type == BulletType::PLAYER_BULLET) {
		this->speed = 650;
		this->power = 450;
	}
	else if (type == BulletType::SLOWER_BULLET) {
		this->speed = 180;
		this->power = 200;
	}
	else if (type == BulletType::FASTER_BULLET) {
		this->speed = FASTER;
		this->power = 300;
	}
	else {
		this->speed = 220;
		this->power = 900;
	}
	
}

void Bullet::move(float frameTime) {
	this->position.y -=  this->speed * frameTime;
}

void Bullet::draw(void) {

	Color tint = WHITE;

	if (this->position.y < 10)								// Player's bullet ROOF
		tint = RED;
	else if (this->position.y > GetScreenHeight() * 0.65)	// Enemies's bullet FLOOR
		tint = GREEN;

	DrawTexture(this->exploding ? this->bulletExplodind_T : this->bullet_T, this->position.x, this->position.y, tint);
}

Bullet* Bullet::StartExploding(void) {
	this->exploding = true;
	return this;
}