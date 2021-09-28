#pragma once
#include "Actor.h"
#include "Game.h"

class Bomb : public Actor
{
public:
	Bomb(float x, float y, Game* game);

	void update();

};

