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
	this->speed = 500;
}

void Bullet::move(float frameTime) {
	this->position.y -=  this->speed * frameTime;
	this->speed += 20;
}

void Bullet::draw(void) {
	DrawTexture(this->exploding ? this->bulletExplodind_T : this->bullet_T, this->position.x, this->position.y, WHITE);
}

Bullet* Bullet::StartExploding(void) {
	this->exploding = true;
	return this;
}