#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Coin.h"
#include "Bomb.h"

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
	list<Coin*> coins;
	list<Bomb*> bombs;

	Player* player1;
	Player* player2;

	Background* background;
	Actor* backgroundPoints;
	Actor* backgroundLifes;

	Text* textPoints;
	Text* textLifes1;
	Text* textLifes2;
	int points;
	
	bool controlShoot1 = false;
	int controlMoveY1 = 0;
	int controlMoveX1 = 0;

	bool controlShoot2 = false;
	int controlMoveY2 = 0;
	int controlMoveX2 = 0;
};


