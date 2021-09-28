#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
	//llama al constructor del padre : Layer(renderer)
	init();
}

void GameLayer::init() {

	audioBackground = new Audio("res/musica_ambiente.mp3", true);
	audioBackground->play();

	//Puntos
	points = 0;
	textPoints = new Text("0", WIDTH * 0.90, HEIGHT * 0.05, game);
	textLifes = new Text("3", WIDTH * 0.93, HEIGHT * 0.85, game);

	player = new Player(50, 50, game); 
	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, -1,game);
	backgroundPoints = new Actor("res/icono_puntos.png",WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);
	backgroundLifes = new Actor("res/corazon.png", WIDTH * (0.85), HEIGHT * (0.85), 44, 36, game);

	playerProjectiles.clear(); //Vaciar por si reiniciamos el juego
	enemyProjectiles.clear();

	enemies.clear(); // Vaciar por si reiniciamos el juego
	enemies.push_back(new Enemy(300, 50, game));
	enemies.push_back(new Enemy(300, 200, game));
	coins.push_back(new Coin(400, 150, game));


}

void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		keysToControls(event);
	}
	//procesar controles
	// Disparar
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();
		if (newProjectile != NULL) {
			playerProjectiles.push_back(newProjectile);
		}

	}

	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	if (controlMoveY > 0) {
		player->moveY(1);
	}
	else if (controlMoveY < 0) {
		player->moveY(-1);
	}
	else {
		player->moveY(0);
	}

}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_SPACE: // dispara
			controlShoot = true;
			break;
		}

	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		}

	}

}

void GameLayer::update() {

	list<Enemy*> deleteEnemies;
	list<Projectile*> deletePlayerProjectiles;
	list<Projectile*> deleteEnemyProjectiles;
	list<Coin*> deleteCoins;

	background->update();

	// Generar enemigos
	newEnemyTime--;
	if (newEnemyTime <= 0) {
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		enemies.push_back(new Enemy(rX, rY, game));
		rX = (rand() % (600 - 500)) + 1 + 500;
		rY = (rand() % (300 - 60)) + 1 + 60;
		coins.push_back(new Coin(rX, rY, game));
		newEnemyTime = 110;

	}



	// Colisiones moneda - player
	for (auto const& coin : coins) {
		coin->update();
		if (player->isOverlap(coin)) {

			points++;
			textPoints->content = to_string(points);

			//Elimina la coin
			bool cInList = std::find(deleteCoins.begin(),
				deleteCoins.end(),
				coin) != deleteCoins.end();

			if (!cInList) {
				deleteCoins.push_back(coin);

			}

		}
	}
	


	player->update();
	for (auto const& enemy : enemies) {

		enemy->update();

		if (enemy->isInRender()) {
			Projectile* newProjectile = enemy->shoot();
			if (newProjectile != NULL) {
				enemyProjectiles.push_back(newProjectile);
			}
		}

	}

	for (auto const& projectile : enemyProjectiles) {
		projectile->update();
	}

	for (auto const& projectile : playerProjectiles) {
		projectile->update();
	}

	// Colisiones Enemigo - Jugador
	for (auto const& enemy : enemies) {
		if (player->isOverlap(enemy)) {
			if (player->numberOfLifes == 1) {
				init();
				return; // Cortar el for
			}
			else {
				
				player->numberOfLifes--;
				textLifes->content = to_string(player->numberOfLifes);

				//Elimina el enemigo
				bool eInList = std::find(deleteEnemies.begin(),
					deleteEnemies.end(),
					enemy) != deleteEnemies.end();

				if (!eInList) {
					deleteEnemies.push_back(enemy);

				}
			}
		}
	}

	//Colisiones Jugador - Proyectil Enemigo
	for (auto const& projectile : enemyProjectiles) {
		if (player->isOverlap(projectile)) {
			if (player->numberOfLifes == 1) {
				init();
				return; // Cortar el for
			}
			else {
				player->numberOfLifes--;
				textLifes->content = to_string(player->numberOfLifes);

				//Disparos de los enemigos 
				bool peInList = std::find(deleteEnemyProjectiles.begin(),
					deleteEnemyProjectiles.end(),
					projectile) != deleteEnemyProjectiles.end();
				if (!peInList) {
					deleteEnemyProjectiles.push_back(projectile);
				}
			}
		}
	}

	//Elimina disparos de player fuera de render
	for (auto const& projectile : playerProjectiles) {
		if (projectile->isInRender() == false) {

			//Disparos del jugador
			bool pInList = std::find(deletePlayerProjectiles.begin(),
				deletePlayerProjectiles.end(),
				projectile) != deletePlayerProjectiles.end();

			if (!pInList) {
				deletePlayerProjectiles.push_back(projectile);
			}
		}
	}

	//Elimina disparos de enemy fuera de render
	for (auto const& projectile : enemyProjectiles) {
		if (projectile->isInRender() == false) {
			//Disparos de los enemigos 
			bool peInList = std::find(deleteEnemyProjectiles.begin(),
				deleteEnemyProjectiles.end(),
				projectile) != deleteEnemyProjectiles.end();

			if (!peInList) {
				deleteEnemyProjectiles.push_back(projectile);
			}
		}
	}

	// Colisiones , Enemy - Player Projectile
	for (auto const& enemy : enemies) {
		for (auto const& projectile : playerProjectiles) {
			if (enemy->isOverlap(projectile)) {

				bool pInList = std::find(deletePlayerProjectiles.begin(),
					deletePlayerProjectiles.end(),
					projectile) != deletePlayerProjectiles.end();

				if (!pInList) {
					deletePlayerProjectiles.push_back(projectile);
				}

				bool eInList = std::find(deleteEnemies.begin(),
					deleteEnemies.end(),
					enemy) != deleteEnemies.end();

				if (!eInList) {
					deleteEnemies.push_back(enemy);

					points++;
					textPoints->content = to_string(points);
				}


			}
		}
	}

	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
	}
	deleteEnemies.clear();

	for (auto const& delCoin : deleteCoins) {
		coins.remove(delCoin);
	}
	deleteCoins.clear();

	for (auto const& delProjectile : deletePlayerProjectiles) {
		playerProjectiles.remove(delProjectile);
		delete delProjectile;
	}
	deletePlayerProjectiles.clear();

	for (auto const& delProjectile : deleteEnemyProjectiles) {
		enemyProjectiles.remove(delProjectile);
		delete delProjectile;
	}
	deleteEnemyProjectiles.clear();

	cout << "update GameLayer" << endl;


}


void GameLayer::draw() {
	background->draw();

	for (auto const& projectile : playerProjectiles) {
		projectile->draw();
	}

	//Dibuja los disparos de los enemigos
	for (auto const& projectile : enemyProjectiles) {
		projectile->draw();
	}

	player->draw();

	for (auto const& coin : coins) {
		coin->draw();
	}

	for (auto const& enemy : enemies) {
		enemy->draw();
	}

	textPoints->draw();
	backgroundPoints->draw();
	textLifes->draw();
	backgroundLifes->draw();

	SDL_RenderPresent(game->renderer); // Renderiza
}


