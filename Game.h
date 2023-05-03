#pragma once
#include "Angel.h"
#include "Model.h"
#include "Object.h"
using namespace std;

class Game {
private:
	GLuint program;
	Model* model;
	Character* character;
	Cube* cube;
	Star* star;
	Fireball* fireball;
	Mushroom* mushroom;
	bool onGame;
public:
	vec3 scale;
	float speed;
	float baseline, initBaseline, nextBaseline;
	bool isMushroomAhead;

	GLuint ver_loc; /* attribute variable */
	GLuint model_loc; /* uniform variable */

	Game();
	void shaderInit();
	void init();
	void drawAll();
	void setOnGame(bool);

	bool getOnGame() { return onGame; }
	GLuint getProgram() { return program; }
	Model* getModel() { return model; }
	Character* getCharacter() { return character; }
	Fireball* getFireball() { return fireball; }
};