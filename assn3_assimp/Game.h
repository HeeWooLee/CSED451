#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Character1.h"
class Game {
private:
	GLuint score = 0;
	bool onGame = true;
	Character character;
	int anime = 0;
public:
	Game();
	void init();
	void endGame();
	void draw();
	void update();
	void setJump();
};