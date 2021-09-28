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
	textLifes1 = new Text("3", WIDTH * 0.93, HEIGHT * 0.75, game);
	textLifes2 = new Text("3", WIDTH * 0.93, HEIGHT * 0.85, game);

	player1 = new Player(50, 50, game); 
	player2 = new Player(50, 200, game);
	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, -1,game);
	backgroundPoints = new Actor("res/icono_puntos.png",WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);
	backgroundLifes = new Actor("res/corazon.png", WIDTH * (0.85), HEIGHT * (0.81), 44, 36, game);

	playerProjectiles.clear(); //Vaciar por si reiniciamos el juego
	enemyProjectiles.clear();

	enemies.clear(); // Vaciar por si reiniciamos el juego
	enemies.push_back(new Enemy(300, 50, game));
	enemies.push_back(new Enemy(300, 200, game));
	coins.push_back(new Coin(400, 150, game));
	bombs.push_back(new Bomb(490, 100, game));

}

void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		keysToControls(event);
	}

	//procesar controles
	// Disparar
	if (controlShoot1) {
		Projectile* newProjectile = player1->shoot();
		if (newProjectile != NULL) {
			playerProjectiles.push_back(newProjectile);
		}
	}

	// Disparar
	if (controlShoot2) {
		Projectile* newProjectile = player2->shoot();
		if (newProjectile != NULL) {
			playerProjectiles.push_back(newProjectile);
		}

	}

	// Eje X
	if (controlMoveX1 > 0) {
		player1->moveX(1);
	}
	else if (controlMoveX1 < 0) {
		player1->moveX(-1);
	}
	else {
		player1->moveX(0);
	}

	// Eje Y
	if (controlMoveY1 > 0) {
		player1->moveY(1);
	}
	else if (controlMoveY1 < 0) {
		player1->moveY(-1);
	}
	else {
		player1->moveY(0);
	}



	// Eje X
	if (controlMoveX2 > 0) {
		player2->moveX(1);
	}
	else if (controlMoveX2 < 0) {
		player2->moveX(-1);
	}
	else {
		player2->moveX(0);
	}

	// Eje Y
	if (controlMoveY2 > 0) {
		player2->moveY(1);
	}
	else if (controlMoveY2 < 0) {
		player2->moveY(-1);
	}
	else {
		player2->moveY(0);
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
			controlMoveX1 = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX1 = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY1 = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY1 = 1;
			break;
		case SDLK_SPACE: // dispara
			controlShoot1 = true;
			break;


		case SDLK_RIGHT: // derecha
			controlMoveX2 = 1;
			break;
		case SDLK_LEFT: // izquierda
			controlMoveX2 = -1;
			break;
		case SDLK_UP: // arriba
			controlMoveY2 = -1;
			break;
		case SDLK_DOWN: // abajo
			controlMoveY2 = 1;
			break;
		case SDLK_PLUS: // dispara
			controlShoot2 = true;
			break;
		}

	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX1 == 1) {
				controlMoveX1 = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX1 == -1) {
				controlMoveX1 = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY1 == -1) {
				controlMoveY1 = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY1 == 1) {
				controlMoveY1 = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot1 = false;
			break;


		case SDLK_RIGHT: // derecha
			if (controlMoveX2 == 1) {
				controlMoveX2 = 0;
			}
			break;
		case SDLK_LEFT: // izquierda
			if (controlMoveX2 == -1) {
				controlMoveX2 = 0;
			}
			break;
		case SDLK_UP: // arriba
			if (controlMoveY2 == -1) {
				controlMoveY2 = 0;
			}
			break;
		case SDLK_DOWN: // abajo
			if (controlMoveY2 == 1) {
				controlMoveY2 = 0;
			}
			break;
		case SDLK_PLUS: // dispara
			controlShoot2 = false;
			break;
		}

		
	}

}

void GameLayer::update() {

	list<Enemy*> deleteEnemies;
	list<Projectile*> deletePlayerProjectiles;
	list<Projectile*> deleteEnemyProjectiles;
	list<Coin*> deleteCoins;
	list<Bomb*> deleteBombs;

	background->update();

	// Generar enemigos, monedas y bombas
	newEnemyTime--;
	if (newEnemyTime <= 0) {
		
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		enemies.push_back(new Enemy(rX, rY, game));
		
		rX = (rand() % (600 - 500)) + 1 + 500;
		rY = (rand() % (300 - 60)) + 1 + 60;
		//Para que no se sature de monedas
		if(coins.size() <= 2)
			coins.push_back(new Coin(rX, rY, game));

		rX = (rand() % (600 - 500)) + 1 + 500;
		rY = (rand() % (300 - 60)) + 1 + 60;
		//Para que no se sature de bombas la pantalla se añade una sola por pantalla
		if(bombs.empty())
			bombs.push_back(new Bomb(rX, rY, game));

		newEnemyTime = 110;

	}



	// Colisiones moneda - player
	for (auto const& coin : coins) {
		coin->update();
		if (player1->isOverlap(coin) || player2->isOverlap(coin)) {

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
	
	// Colisiones bomb - player
	for (auto const& bomb : bombs) {
		bomb->update();
		if (player1->isOverlap(bomb) || player2->isOverlap(bomb)) {


			//Elimina la bomb
			bool bInList = std::find(deleteBombs.begin(),
				deleteBombs.end(),
				bomb) != deleteBombs.end();

			if (!bInList) {
				deleteBombs.push_back(bomb);

			}

			//Elimina todos los enemigos
			enemies.clear();

		}
	}

	player1->update();
	player2->update();
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
		if (player1->isOverlap(enemy) || player2 -> isOverlap(enemy)) {
			if ((player1->numberOfLifes == 1 && player1->isOverlap(enemy)) || (player2->numberOfLifes == 1 && player2->isOverlap(enemy))) {
				init();
				return; // Cortar el for
			}
			if (player1->isOverlap(enemy)){
				
				player1->numberOfLifes--;
				textLifes1->content = to_string(player1->numberOfLifes);

				//Elimina el enemigo
				bool eInList = std::find(deleteEnemies.begin(),
					deleteEnemies.end(),
					enemy) != deleteEnemies.end();

				if (!eInList) {
					deleteEnemies.push_back(enemy);

				}
			}

			if (player2->isOverlap(enemy)) {
				player2->numberOfLifes--;
				textLifes2->content = to_string(player2->numberOfLifes);

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
		if (player1->isOverlap(projectile) || player2->isOverlap(projectile)) {
			if ((player1->numberOfLifes == 1 && player1->isOverlap(projectile)) || (player2->numberOfLifes == 1 && player2->isOverlap(projectile))) {
				init();
				return; // Cortar el for
			}
			if (player1->isOverlap(projectile)) {
				player1->numberOfLifes--;
				textLifes1->content = to_string(player1->numberOfLifes);

				//Disparos de los enemigos 
				bool peInList = std::find(deleteEnemyProjectiles.begin(),
					deleteEnemyProjectiles.end(),
					projectile) != deleteEnemyProjectiles.end();
				if (!peInList) {
					deleteEnemyProjectiles.push_back(projectile);
				}
			}

			if (player2->isOverlap(projectile)) {
				player2->numberOfLifes--;
				textLifes2->content = to_string(player2->numberOfLifes);

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

	for (auto const& delBomb : deleteBombs) {
		bombs.remove(delBomb);
	}
	deleteBombs.clear();

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

	player1->draw();
	player2->draw();
	

	for (auto const& coin : coins) {
		coin->draw();
	}

	for (auto const& bomb : bombs) {
		bomb->draw();
	}

	for (auto const& enemy : enemies) {
		enemy->draw();
	}

	textPoints->draw();
	backgroundPoints->draw();
	textLifes1->draw();
	textLifes2->draw();
	backgroundLifes->draw();

	SDL_RenderPresent(game->renderer); // Renderiza
}


