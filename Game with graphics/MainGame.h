#pragma once

#include "Sprite.h"
#include "GLSLProgram.h"

#include <SDL\SDL.h>
#include <GL\glew.h>

enum class GAMESTATE {CONTINUE, QUIT};

class MainGame
{
public:
	MainGame();
	int run();

private:

	void initSystems();
	void initShaders();

	int gameLoop();
	void processInput();
	void drawGame();

	SDL_Window* _window;
	GAMESTATE _currentGamestate;

	struct WindowParameters {
		char* _winName = "Game Window";
		int _winX = SDL_WINDOWPOS_CENTERED;
		int _winY = SDL_WINDOWPOS_CENTERED;
		int _winWidth = 1024;
		int _winHeight = 768;
		int _flags = SDL_WINDOW_OPENGL;
	} _windowParameters;

	Sprite _sprite;

	GLSLProgram _colorShaderProgram;
};

