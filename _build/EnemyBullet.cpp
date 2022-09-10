#include "EnemyBullet.h"

EnemyBullet::EnemyBullet(Texture2D enemyB_t1, Texture2D enemyB_t2, Texture2D enemyB_t3, Texture2D enemyB_t4, Texture2D enemyExplodingB_t,
						Position spawn, BulletType type, bool exploding)
	: Bullet(enemyB_t1 ,enemyB_t1, spawn, type, exploding) {
	
	this->eBullet_T1 = enemyB_t1;
	this->eBullet_T2 = enemyB_t2;
	this->eBullet_T3 = enemyB_t3;
	this->eBullet_T4 = enemyB_t4;

	this->eBulletExploding_T = enemyExplodingB_t;
}

void EnemyBullet::draw(void) {




	//DrawTexture(this->exploding ? this->bulletExplodind_T : this->bullet_T, this->position.x, this->position.y, WHITE);
}