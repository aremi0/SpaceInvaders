#include "EnemyBullet.h"

EnemyBullet::EnemyBullet(Texture2D enemyB_t1, Texture2D enemyB_t2, Texture2D enemyB_t3, Texture2D enemyB_t4, Texture2D enemyExplodingB_t,
						Position spawn, BulletType type, bool exploding)
	: Bullet(enemyB_t1 , enemyExplodingB_t, spawn, type, exploding) {
	
	this->speed = 100;
	this->movementState = 0;

	this->eBullet_T1 = enemyB_t1;
	this->eBullet_T2 = enemyB_t2;
	this->eBullet_T3 = enemyB_t3;
	this->eBullet_T4 = enemyB_t4;
}

// TODO asd
void EnemyBullet::draw(void) {

	if(exploding)
		DrawTexture(this->bulletExplodind_T, this->position.x, this->position.y, WHITE);
	else {
		switch (movementState)
		{
			case 0:
				DrawTexture(this->eBullet_T1, this->position.x, this->position.y, WHITE);
				break;
			case 1:
				DrawTexture(this->eBullet_T2, this->position.x, this->position.y, WHITE);
				break;
			case 2:
				DrawTexture(this->eBullet_T3, this->position.x, this->position.y, WHITE);
				break;
			case 3:
				DrawTexture(this->eBullet_T4, this->position.x, this->position.y, WHITE);
				break;
		}
	}
}

void EnemyBullet::move(float frameTime) {

	this->position.y +=  this->speed * frameTime;

	if (++movementState == 4)
		movementState = 0;
}