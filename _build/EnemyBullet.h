#pragma once
#include "Bullet.h"

class EnemyBullet : public Bullet {
public:
// Data
	unsigned movementState;

// Textures
	// Moving Animation Texture
	Texture2D eBullet_T1;
	Texture2D eBullet_T2;
	Texture2D eBullet_T3;
	Texture2D eBullet_T4;

// Methods
	// Richiama costruttore classe base e poi inizializza nuove texture/variabili
	EnemyBullet(Texture2D enemyB_t1, Texture2D enemyB_t2, Texture2D enemyB_t3, Texture2D enemyB_t4, Texture2D enemyExplodingB_t, Position spawn, BulletType type, bool exploding = false);
	void draw(void); //Overload from base-class
	void move(float frameTime);
};

