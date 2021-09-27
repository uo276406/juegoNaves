#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Text.h"

#include "Audio.h"

#include <list>

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;

	Audio* audioBackground;

	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);
	int newEnemyTime = 0;

	list<Enemy*> enemies;
	list<Projectile*> playerProjectiles;
	list<Projectile*> enemyProjectiles;

	Player* player;
	Background* background;
	Actor* backgroundPoints;
	Text* textPoints;
	int points;
	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;
};


