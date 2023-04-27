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
#include "Utils.h"
#include "GameObject.h"
class Game {
private:
	GLuint score = 0;
	bool onGame = true;
	Character character;
	Terrain terrain;
	Star* star;
	int anime = 0;
	GLfloat gameSpeed;
	GLfloat scale = 1000.0f;

	std::vector<Position> starPattern = { {0.1f, 0.5f}, {0.2f, 0.6f}, {0.3f, 0.7f}, {0.4f, 0.8f}, {0.5f, 0.7f}, {0.6f, 0.6f}, {0.7f, 0.5f} };

	std::vector<Position> starPattern2 = { {0.0f, 0.5f}, {0.4f, 0.5f}, {0.6f, 0.5f} };
	std::vector<std::vector<Position>> starPatterns = { starPattern, starPattern2 };


	std::vector<Position> fireballPattern = { {0.1f, 0.9f},    {0.7f, 0.7f} };
	std::vector<Position> fireballPattern2 = { {0.0f, 0.7f}, {0.6f, 0.5f} };
	std::vector<std::vector<Position>> fireballPatterns = { fireballPattern, fireballPattern2 };
	std::vector<GLfloat> stars; GLfloat starPosX = 0.0f;
	std::vector<GLfloat> starsPatternChoice;
	std::vector<std::vector<GLfloat>> starsConsumed;


	Position fireballPos;
	GLfloat fireballPosX;
	GLfloat fireballSpeed = 2.0 * gameSpeed;
	std::vector<Position> fireballs;
	int fireballWidth, fireballHeight, nrChannels;
public:
	Game();
	void init();
	void endGame();
	void draw();
	void update();
	void setJump(); void downScale(); void upScale();

	void starContactDetected();
	void drawText(std::string text, void* font);
	void drawScore();
};