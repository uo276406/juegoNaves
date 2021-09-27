#include "Projectile.h"

Projectile::Projectile(string image, float x, float y, int width, int height, Game* game, int vx) :
	Actor(image, x, y, width, height, game) {
	this->vx = vx;
}

void Projectile::update() {
	x = x + vx;
}

