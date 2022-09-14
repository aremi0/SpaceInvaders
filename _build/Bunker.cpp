#include "Bunker.h"

Bunker::Bunker(Position sp,
				Texture2D tL1, Texture2D tL2, Texture2D tL3, Texture2D tL4,
				Texture2D c1, Texture2D c2, Texture2D c3, Texture2D c4,
				Texture2D tR1, Texture2D tR2, Texture2D tR3, Texture2D tR4,
				Texture2D bL1, Texture2D bL2, Texture2D bL3, Texture2D bL4,
				Texture2D bR1, Texture2D bR2, Texture2D bR3, Texture2D bR4) {

	this->spawn = sp;

	this->topLeft = new BunkerSlice(2600, tL1, tL2, tL3, tL4);
	this->topCenter = new BunkerSlice(2900, c1, c2, c3, c4);
	this->topRight = new BunkerSlice(2600, tR1, tR2, tR3, tR4);

	this->centerLeft = new BunkerSlice(2900, c1, c2, c3, c4);
	this->centerCenter = new BunkerSlice(2900, c1, c2, c3, c4);
	this->centerRight = new BunkerSlice(2900, c1, c2, c3, c4);

	this->bottomLeft = new BunkerSlice(1200, bL1, bL2, bL3, bL4);
	this->bottomRight = new BunkerSlice(1200, bR1, bR2, bR3, bR4);

	this->health = 8;
}

bool Bunker::collisionDetector(Bullet* bullet) {


	// TOP-LEFT
	if (topLeft) {
		if (CheckCollisionRecs(Rectangle{ this->spawn.x, this->spawn.y, this->topLeft->state1.width * 1.0f, this->topLeft->state1.height * 1.0f },
			Rectangle{ bullet->position.x, bullet->position.y, bullet->bullet_T.width * 1.0f, bullet->bullet_T.height * 1.0f })) {

			this->topLeft->currentHealth -= bullet->power;

			printf("___bunker__HIT__<b.type:%d><top-left:%d>______\n", (int)bullet->type, this->topLeft->currentHealth);

			if (bullet->type == BulletType::PLAYER_BULLET)
				bullet->position.y += 5;

			if (topLeft->currentHealth < 10) {
				delete topLeft;
				topLeft = nullptr;
				this->health--;
			}

			return true;
		}
	}

	// TOP-CENTER
	if (topCenter) {
		if (CheckCollisionRecs(Rectangle{ this->spawn.x + this->topCenter->state1.width, this->spawn.y, this->topCenter->state1.width * 1.0f, this->topCenter->state1.height * 1.0f },
			Rectangle{ bullet->position.x, bullet->position.y, bullet->bullet_T.width * 1.0f, bullet->bullet_T.height * 1.0f })) {

			this->topCenter->currentHealth -= bullet->power;
			this->health -= bullet->power;

			printf("___bunker__HIT__<b.type:%d><top-center:%d>______\n", (int)bullet->type, this->topLeft->currentHealth);

			if (bullet->type == BulletType::PLAYER_BULLET)
				bullet->position.y += 5;

			if (topCenter->currentHealth < 10) {
				delete topCenter;
				topCenter = nullptr;
			}

			return true;
		}
	}

	// TOP-RIGHT
	if (topRight) {
		if (CheckCollisionRecs(Rectangle{ this->spawn.x + this->topRight->state1.width * 2, this->spawn.y, this->topRight->state1.width * 1.0f, this->topRight->state1.height * 1.0f },
			Rectangle{ bullet->position.x, bullet->position.y, bullet->bullet_T.width * 1.0f, bullet->bullet_T.height * 1.0f })) {

			this->topRight->currentHealth -= bullet->power;
			this->health -= bullet->power;

			printf("___bunker__HIT__<b.type:%d><top-right:%d>______\n", (int)bullet->type, this->topLeft->currentHealth);

			if (bullet->type == BulletType::PLAYER_BULLET)
				bullet->position.y += 5;

			if (topRight->currentHealth < 10) {
				delete topRight;
				topRight = nullptr;
			}

			return true;
		}
	}

	// CENTER-LEFT
	if (centerLeft) {
		if (CheckCollisionRecs(Rectangle{ this->spawn.x, this->spawn.y + this->centerLeft->state1.height, this->centerLeft->state1.width * 1.0f, this->centerLeft->state1.height * 1.0f },
			Rectangle{ bullet->position.x, bullet->position.y, bullet->bullet_T.width * 1.0f, bullet->bullet_T.height * 1.0f })) {

			this->centerLeft->currentHealth -= bullet->power;
			this->health -= bullet->power;

			printf("___bunker__HIT__<b.type:%d><center-left:%d>______\n", (int)bullet->type, this->topLeft->currentHealth);

			if (bullet->type == BulletType::PLAYER_BULLET)
				bullet->position.y += 5;

			if (centerLeft->currentHealth < 10) {
				delete centerLeft;
				centerLeft = nullptr;
			}

			return true;
		}
	}

	// CENTER-CENTER
	if (centerCenter) {
		if (CheckCollisionRecs(Rectangle{ this->spawn.x + this->centerCenter->state1.width, this->spawn.y + this->centerCenter->state1.height, this->centerCenter->state1.width * 1.0f, this->centerCenter->state1.height * 1.0f },
			Rectangle{ bullet->position.x, bullet->position.y, bullet->bullet_T.width * 1.0f, bullet->bullet_T.height * 1.0f })) {

			this->centerCenter->currentHealth -= bullet->power;
			this->health -= bullet->power;

			printf("___bunker__HIT__<b.type:%d><center-center:%d>______\n", (int)bullet->type, this->topLeft->currentHealth);
			
			if (bullet->type == BulletType::PLAYER_BULLET)
				bullet->position.y += 5;

			if (centerCenter->currentHealth < 10) {
				delete centerCenter;
				centerCenter = nullptr;
			}

			return true;
		}
	}

	// CENTER-RIGHT
	if (centerRight) {
		if (CheckCollisionRecs(Rectangle{ this->spawn.x + this->centerRight->state1.width * 2, this->spawn.y + this->centerRight->state1.height, this->centerRight->state1.width * 1.0f, this->centerRight->state1.height * 1.0f },
			Rectangle{ bullet->position.x, bullet->position.y, bullet->bullet_T.width * 1.0f, bullet->bullet_T.height * 1.0f })) {

			this->centerRight->currentHealth -= bullet->power;
			this->health -= bullet->power;

			printf("___bunker__HIT__<b.type:%d><center-right:%d>______\n", (int)bullet->type, this->topLeft->currentHealth);

			if (bullet->type == BulletType::PLAYER_BULLET)
				bullet->position.y += 5;

			if (centerRight->currentHealth < 10) {
				delete centerRight;
				centerRight = nullptr;
			}

			return true;
		}
	}

	// BOTTOM-LEFT
	if (bottomLeft) {
		if (CheckCollisionRecs(Rectangle{ this->spawn.x, this->spawn.y + this->bottomLeft->state1.height * 2, this->bottomLeft->state1.width * 1.0f, this->bottomLeft->state1.height - 22 * 1.0f },
			Rectangle{ bullet->position.x, bullet->position.y, bullet->bullet_T.width * 1.0f, bullet->bullet_T.height * 1.0f })) {

			this->bottomLeft->currentHealth -= bullet->power;
			this->health -= bullet->power;

			printf("___bunker__HIT__<b.type:%d><bottom-left:%d>______\n", (int)bullet->type, this->topLeft->currentHealth);

			if (bottomLeft->currentHealth < 10) {
				delete bottomLeft;
				bottomLeft = nullptr;
			}

			return true;
		}
	}

	// BOTTOM-RIGHT
	if (bottomRight) {
		if (CheckCollisionRecs(Rectangle{ this->spawn.x + this->bottomRight->state1.width * 2, this->spawn.y + this->bottomRight->state1.height * 2, this->bottomRight->state1.width * 1.0f, this->bottomRight->state1.height - 22 * 1.0f },
			Rectangle{ bullet->position.x, bullet->position.y, bullet->bullet_T.width * 1.0f, bullet->bullet_T.height * 1.0f })) {

			this->bottomRight->currentHealth -= bullet->power;
			this->health -= bullet->power;

			printf("___bunker__HIT__<b.type:%d><bottom-right:%d>______\n", (int)bullet->type, this->topLeft->currentHealth);

			if (bottomRight->currentHealth < 10) {
				delete bottomRight;
				bottomRight = nullptr;
			}

			return true;
		}
	}

	return false;
}

void Bunker::draw(void) {
	
	if(topLeft)
		DrawTexture(topLeft->draw(), this->spawn.x, this->spawn.y, GREEN);
	if(topCenter)
		DrawTexture(topCenter->draw(), this->spawn.x + topCenter->state1.width, this->spawn.y, GREEN);
	if(topRight)
		DrawTexture(topRight->draw(), this->spawn.x + topRight->state1.width * 2, this->spawn.y, GREEN);

	if(centerLeft)
		DrawTexture(centerLeft->draw(), this->spawn.x, this->spawn.y + centerLeft->state1.height, GREEN);
	if(centerCenter)
		DrawTexture(centerCenter->draw(), this->spawn.x + centerCenter->state1.width, this->spawn.y + centerCenter->state1.height, GREEN);
	if(centerRight)
		DrawTexture(centerRight->draw(), this->spawn.x + centerRight->state1.width * 2, this->spawn.y + centerRight->state1.height, GREEN);

	if(bottomLeft)
		DrawTexture(bottomLeft->draw(), this->spawn.x, this->spawn.y + bottomLeft->state1.height + bottomLeft->state1.height, GREEN);
	if(bottomRight)
		DrawTexture(bottomRight->draw(), this->spawn.x + bottomRight->state1.width * 2, this->spawn.y + bottomRight->state1.height * 2, GREEN);
}