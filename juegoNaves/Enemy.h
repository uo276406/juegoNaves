#pragma once

#include "Actor.h"
#include "Animation.h" 
#include "Projectile.h"

class Enemy : public Actor
{
public:
	Enemy(float x, float y, Game* game);
	void draw() override; // Va a sobrescribir
	void update();
	Animation* aMoving;
	Animation* animation; // Referencia a la animación mostrada
	
	Projectile* shoot();
	int shootCadence = 90;
	int shootTime = 0;
};



