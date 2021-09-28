#include "Coin.h"

Coin::Coin(float x, float y, Game* game)
	: Actor("res/moneda.png", x, y, 40, 40, game) {

	vx = 1;

}


void Coin::update() {

	vx = -1;
	x = x + vx;

}

