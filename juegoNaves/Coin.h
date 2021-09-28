#include "Game.h"
#include "Actor.h"

#pragma once
class Coin : public Actor
{
public:
	Coin(float x, float y, Game* game);

	void update();

};

