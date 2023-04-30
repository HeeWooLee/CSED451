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
public:
	vec3 scale;
	float speed;
	float baseline, initBaseline, prevBaseline;

	GLuint ver_loc; /* attribute variable */
	GLuint model_loc; /* uniform variable */
	GLuint projection_loc; /* uniform variable */

	Game();
	void shaderInit();
	void init();
	void drawAll();

	GLuint getProgram() { return program; }
	Model* getModel() { return model; }
	Character* getCharacter() { return character; }
};