#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador.png", x, y, 50, 57, game) {

	audioShoot = new Audio("res/efecto_disparo.wav", false);

}

Projectile* Player::shoot() {
	if (shootTime == 0) {
		audioShoot->play();
		shootTime = shootCadence;
		return new Projectile("res/disparo_jugador.png",x, y,18,6, game, 9);
	}
	else {
		return NULL;
	}
}


void Player::update() {

	if (shootTime > 0) {
		shootTime--;
	}

	x = x + vx;
	y = y + vy;
}

void Player::moveX(float axis) {
	vx = axis * 3;
}

void Player::moveY(float axis) {
	vy = axis * 3;
}



