#include "MainGame.h"
#include "Errors.h"

#include <iostream>
#include <string>


////PUBLIC----

MainGame::MainGame() : _window(nullptr), _currentGamestate(GAMESTATE::CONTINUE) { //init important variables
}

int MainGame::run() { //function called from main to run the game

	initSystems();
	_sprite.init(-0.2f, -0.2f, 0.4f, 0.4f);
	gameLoop();
	return 0;
}


////PRIVATE----

void MainGame::initSystems() { //Initialize stuff

	//Init SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	//create window
	_window = SDL_CreateWindow(_windowParameters._winName, _windowParameters._winX, _windowParameters._winY, _windowParameters._winWidth, _windowParameters._winHeight, _windowParameters._flags);
	if (_window == nullptr) {
		error("_window not initialized (Window failed to create?)", true);
	}

	//create a gl context for the window so openGL can interface well
	SDL_GLContext glContext;;
	glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr) {
		error("SDL_GL_CreateContext failed to create context on window", true);
	}

	//init glew
	GLenum errorCheck = glewInit();
	if (errorCheck != GLEW_OK) {
		error("GLEW failed to initialize properly", true);
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //one frame is drawn on while one stays visible and when finished the top frame is cleared and swapped and the drawing happens on that (no flickering)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //background (R,G,B,Alpha) - what color is the window is when cleared

	initShaders();
}


void MainGame::initShaders() {

	_colorShaderProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorShaderProgram.addAttribute("vertexPositoin");
	_colorShaderProgram.linkShaders();
}


int MainGame::gameLoop() { //main game loop - stops when users quits

	//Loop game untill user exits
	while (_currentGamestate != GAMESTATE::QUIT) {
		processInput();
		drawGame();
	}
	return 0;
}



void MainGame::processInput() { //SDL polls for events and processes them

	//Check for input
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_currentGamestate = GAMESTATE::QUIT;
			return;
		case SDL_MOUSEMOTION:
			/*if (evnt.motion.xrel > 1000) {
				_currentGamestate = GAMESTATE::QUIT;
			}*/

			std::cout << evnt.motion.xrel << std::endl;
			break;
		}
	}
}


void MainGame::drawGame() { //draw to, clear and swap frame

	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorShaderProgram.use();

	_sprite.draw();

	_colorShaderProgram.unUse();

	SDL_GL_SwapWindow(_window);
}
