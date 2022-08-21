#include "Player.h"

Player::Player(Texture2D player_t) {
// Texture init
	this->player_T = player_t;

// Exploding Animation init

// Parameter init
	this->hardReset();
	this->position.y = GetScreenHeight() * 0.90f; // Posiziona il player al 90% dell'altezza della 
	this->speed = 300;
	this->exploding = false;
}

/*
	shift positivo o negatavo; permette di muovere il player orizzontalmente. Si occuperà screen_gameplay di controllare se il movimento e' legale
*/
void Player::move(Movement direction, float frameTime) {
	this->position.x += (direction * this->speed * frameTime);
}

/*
	Resetta solo le variabili tali da poter affrontare una nuova ondata (next-level)
*/
void Player::reset() {
	this->position.x = PLAYER_SPAWNPOINT;
}

/*
	Resetta tutte le variabili per eventualmente ricominciare un nuovo game
*/
void Player::hardReset() {
	this->life = 3;
	this->reset();
}

void Player::draw() {
	DrawTexture(this->exploding ? this->playerExploding_T : this->player_T, this->position.x, this->position.y, WHITE);
}