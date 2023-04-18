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
#include "Terrain.h"
class Game {
private:
	GLuint score = 0;
	bool onGame = true;
	Character character;
	Terrain terrain;
	int anime = 0;
	GLfloat gameSpeed;
public:
	Game();
	void init();
	void endGame();
	void draw();
	void update();
	void setJump();

};