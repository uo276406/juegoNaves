#pragma once

// Todas las librerias de SDL y otras utilidades
#include <iostream>
#include <string>
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <map> 

#include "Layer.h"
class Layer;

// Valores generales
#define WIDTH 480
#define HEIGHT 320

class Game
{
public:
	Game();
	void loop();

	void scale();
	SDL_Texture* getTexture(string filename);
	map<string, SDL_Texture*> mapTextures; // map - cache

	TTF_Font* font;

	bool scaledToMax = false;
	float scaleLower = 1;

	SDL_Window* window; // ventana
	SDL_Renderer* renderer; // renderizador
	bool loopActive; // Juego activo

	Layer* gameLayer;
};




