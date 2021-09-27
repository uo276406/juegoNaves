#pragma once

#include "Actor.h"

class Projectile : public Actor
{
public:
	Projectile(string image, float x, float y, int width, int height, Game* game, int vx);
	void update();
};


